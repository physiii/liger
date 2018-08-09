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

#define DUMB_PERIOD_US 5000

int dumb_count = 0;
int data_part_count = 0;
char dumb_count_str[10];
char wss_data_in[2000];
char wss_data_out[2000];
bool wss_data_out_ready = false;

//test code

//end of test code

struct pss__wss {
	int number;
};

struct vhd__wss {
	const unsigned int *options;
};

int
add_headers(void *in, size_t len) {
	char **h = (char **)in;

	if (len < 100)
		return 1;

	*h += sprintf(*h, "x-device-id: %s\x0d\x0a",device_id);
	*h += sprintf(*h, "x-device-token: %s\x0d\x0a",token);

	return 0;
}

int
wss_event_handler(cJSON * root) {
	if (cJSON_GetObjectItem(root,"token")) {
		if (token_received) return 0;
		sprintf(token,"%s",cJSON_GetObjectItem(root,"token")->valuestring);
		printf("token received: %s\n", token);
		token_received = true;
		strcpy(wss_data_in,"");
		data_part_count = 0;
		lwsl_notice("protocol_wss: closing as requested\n");
		lws_close_reason(wsi, LWS_CLOSE_STATUS_GOINGAWAY,
			(unsigned char *)"reconnecting with new token in headers", 5);
		wsi_connect = 1;
		store_char("token",token);
		return -1;
	}
}

static int
callback_wss(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	struct pss__wss *pss = (struct pss__wss *)user;
	struct vhd__wss *vhd =
				(struct vhd__wss *)
				lws_protocol_vh_priv_get(lws_get_vhost(wsi),
						lws_get_protocol(wsi));
	uint8_t buf[LWS_PRE + 2000], *p = &buf[LWS_PRE];
	const struct lws_protocol_vhost_options *opt;
	int n, m;

	switch (reason) {
	case LWS_CALLBACK_PROTOCOL_INIT:
		//lwsl_notice("!! ------- wss LWS_CALLBACK_PROTOCOL_INIT ---- !!\n");
		vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
			lws_get_protocol(wsi),
			sizeof(struct vhd__wss));
		if (!vhd)
			return -1;
		if ((opt = lws_pvo_search(
				(const struct lws_protocol_vhost_options *)in,
				"options")))
			vhd->options = (unsigned int *)opt->value;
		break;

		case	LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
			//printf("LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER\n");
			add_headers(in,len);
			break;

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		lws_callback_on_writable(wsi);
		//lws_callback_on_writable_all_protocol_vhost(
		//	lws_get_vhost(wsi), lws_get_protocol(wsi));
		pss->number = 0;
		//token_received = false;
		strcpy(wss_data_in,"");
		if (!vhd->options || !((*vhd->options) & 1))
			lws_set_timer_usecs(wsi, DUMB_PERIOD_US);
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		//printf("LWS_CALLBACK_CLIENT_WRITEABLE\n");

		/*if (dumb_count < 99) dumb_count++;
		else dumb_count = 0;
		char event_type[128] = "buttons";*/

		//sprintf(wss_data_out,"{\"event_type\":\"buttons\", \"payload\":{\"value\":%d}}",dumb_count);
		//sprintf(wss_data_out,"{\"event_type\":\"buttons\"}");

		//strcpy(wss_data_out, );
		//strcat(wss_data_out, dumb_count_str);
		/*strcpy(wss_data_out, "{\"event_type\":\"");
		strcat(wss_data_out,event_type);
		strcat(wss_data_out, "\",\"payload\":{}}\"");
		strcat(wss_data_out,"}");*/
		if (!wss_data_out_ready) break;
		n = lws_snprintf((char *)p, sizeof(wss_data_out) - LWS_PRE, "%s", wss_data_out);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to token socket\n", n);
		} else {
			wss_data_out_ready = false;
			//printf("%s\n",wss_data_out);
		}
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		//printf("!! LWS_CALLBACK_RECEIVE %d",wss_data_in);
		if (len < 6)
			break;

		if (data_part_count > 100) {
			strcpy(wss_data_in,"");
			data_part_count = 0;
		} else data_part_count++;

		strcat(wss_data_in,(const char *)in);
	
    const char *parse_end = NULL;
		bool valid_json = false;
    cJSON *root = cJSON_ParseWithOpts(wss_data_in, &parse_end, true);

		//break if json is not valid, part count because 
		//first part of message is wrongly being seen as valid json
		if (strcmp(parse_end,"") && data_part_count < 2) break;
		
		//printf("wss_data_in: %s\n",wss_data_in);
		//cJSON *root = cJSON_Parse(wss_data_in);
		
		int res = wss_event_handler(root);
		if (res == -1) return -1;
		
		break;

	case LWS_CALLBACK_CLIENT_CLOSED:
		lwsl_notice("protocol_wss: closing as requested\n");
		lws_close_reason(wsi, LWS_CLOSE_STATUS_GOINGAWAY,
			(unsigned char *)"LWS_CALLBACK_CLIENT_CLOSED reconnecting...", 5);
		wsi_connect = 1;
		return -1;
		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		lwsl_notice("protocol_wss: closing as requested\n");
		lws_close_reason(wsi, LWS_CLOSE_STATUS_GOINGAWAY,
			(unsigned char *)"LWS_CALLBACK_CLIENT_CLOSED reconnecting...", 5);
		wsi_connect = 1;
		return -1;
		break;

	case LWS_CALLBACK_TIMER:
		if (!vhd->options || !((*vhd->options) & 1)) {
			lws_callback_on_writable_all_protocol_vhost(
				lws_get_vhost(wsi), lws_get_protocol(wsi));
			lws_set_timer_usecs(wsi, DUMB_PERIOD_US);
		}
		break;

		default:
		   	printf("wss-protocol callback: %d\n",reason);
			break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_WSS \
	{ \
		"wss-protocol", \
		callback_wss, \
		sizeof(struct pss__wss), \
		10, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)

static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_WSS
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_wss(struct lws_context *context,
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
destroy_protocol_wss(struct lws_context *context)
{
	return 0;
}

#endif
