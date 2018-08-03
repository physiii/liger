/*
 * ws protocol handler plugin for "dumb increment"
 *
 * Copyright (C) 2010-2018 Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * The person who associated a work with this deed has dedicated
 * the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related
 * and neighboring rights, to the extent allowed by law. You can copy,
 * modify, distribute and perform the work, even for commercial purposes,
 * all without asking permission.
 *
 * These test plugins are intended to be adapted for use in your code, which
 * may be proprietary.  So unlike the library itself, they are licensed
 * Public Domain.
 */

#if !defined (LWS_PLUGIN_STATIC)
#define LWS_DLL
#define LWS_INTERNAL
#include "../lib/libwebsockets.h"
#endif

#include <string.h>

#define DUMB_PERIOD_US 500 * 1000
char token_req_str[512];
int dumb_count = 0;
char dumb_count_str[10];
bool token_received = false;
const char device_id[100];

struct pss__token {
	int number;
};

struct vhd__token {
	const unsigned int *options;
};

static int
add_headers(void *in, size_t len) {
	char **p = (char **)in;
	
	if (len < 100)
		return 1;
	
	strcpy(device_id,"25dc4876-d1e2-4d6e-ba4f-fba81992c888");
	*p += sprintf(*p, "x-device-id: %s\x0d\x0a",device_id);
	*p += sprintf(*p, "x-device-token: %s\x0d\x0a",device_id);

	return 0;
}


static int
callback_token(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	struct pss__token *pss = (struct pss__token *)user;
	struct vhd__token *vhd =
				(struct vhd__token *)
				lws_protocol_vh_priv_get(lws_get_vhost(wsi),
						lws_get_protocol(wsi));
	uint8_t buf[LWS_PRE + 20], *p = &buf[LWS_PRE];
	const struct lws_protocol_vhost_options *opt;
	int n, m;

	switch (reason) {
	case LWS_CALLBACK_PROTOCOL_INIT:
		//lwsl_notice("token LWS_CALLBACK_PROTOCOL_INIT\n");
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
			lws_get_protocol(wsi),
			sizeof(struct vhd__token));
		if (!vhd)
			return -1;
		if ((opt = lws_pvo_search(
				(const struct lws_protocol_vhost_options *)in,
				"options")))
			vhd->options = (unsigned int *)opt->value;
		break;

	case	LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
		printf("LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER\n");
		int res = add_headers(in,len);
		break;
	
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		lws_callback_on_writable(wsi);
		//lws_callback_on_writable_all_protocol_vhost(
		//	lws_get_vhost(wsi), lws_get_protocol(wsi));
		wsi_connect = false;
		pss->number = 0;
		if (!vhd->options || !((*vhd->options) & 1))
			lws_set_timer_usecs(wsi, DUMB_PERIOD_US);
		break;

		case LWS_CALLBACK_CLIENT_WRITEABLE:
			//printf("LWS_CALLBACK_CLIENT_WRITEABLE\n");
			if (token_received) break;
			strcpy(token_req_str, "{\"event_type\":\"getUUID\"}");
			//strcat(token_req_str, dumb_count_str);

			n = lws_snprintf((char *)p, sizeof(token_req_str) - LWS_PRE, "%s", token_req_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n)
				lwsl_err("ERROR %d writing to token socket\n", n);
			else  {
				//printf("%s\n",token_req_str);
			}
			break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		//token_received = true;
	  printf("%s\n",(const char *)in);
		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		wsi_connect = true;
		printf("LWS_CALLBACK_CLIENT_CONNECTION_ERROR wsi_connect=true \n");
		break;
		
	case LWS_CALLBACK_CLIENT_CLOSED:
		wsi_connect = true;
		printf("LWS_CALLBACK_CLIENT_CLOSED wsi_connect=true \n");
		break;
		
	case LWS_CALLBACK_TIMER:
		if (!vhd->options || !((*vhd->options) & 1)) {
			lws_callback_on_writable_all_protocol_vhost(
				lws_get_vhost(wsi), lws_get_protocol(wsi));
			lws_set_timer_usecs(wsi, DUMB_PERIOD_US);
		}
		break;

		default:
		   	printf("callback_test-protocol: %d\n",reason);
			break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_TOKEN \
	{ \
		"token-protocol", \
		callback_token, \
		sizeof(struct pss__token), \
		10, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)

static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_TOKEN
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_token(struct lws_context *context,
			     struct lws_plugin_capability *c)
{
	if (c->api_magic != LWS_PLUGIN_API_MAGIC) {
		lwsl_err("Plugin API %d, library API %d", LWS_PLUGIN_API_MAGIC,
			 c->api_magic);
		return 1;
	}

	c->protocols = protocols;
	c->count_protocols = ARRAY_SIZE(protocols);
	c->extensions = NULL;
	c->count_extensions = 0;

	return 0;
}

LWS_EXTERN LWS_VISIBLE int
destroy_protocol_token(struct lws_context *context)
{
	return 0;
}

#endif
