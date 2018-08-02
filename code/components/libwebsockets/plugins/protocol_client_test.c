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

/* test code */
#include <signal.h>
#include <pthread.h>


static int interrupted;

/* one of these created for each message */

struct msg {
	void *payload; /* is malloc'd */
	size_t len;
};

struct per_vhost_data__minimal {
	struct lws_context *context;
	struct lws_vhost *vhost;
	const struct lws_protocols *protocol;
	pthread_t pthread_spam[2];

	pthread_mutex_t lock_ring; /* serialize access to the ring buffer */
	struct lws_ring *ring; /* ringbuffer holding unsent messages */
	uint32_t tail;

	struct lws_client_connect_info i;
	struct lws *client_wsi;

	int counter;
	char finished;
	char established;
};

static void
__minimal_destroy_message(void *_msg)
{
	struct msg *msg = _msg;

	free(msg->payload);
	msg->payload = NULL;
	msg->len = 0;
}


static void
thread_spam(void *d)
{
	struct per_vhost_data__minimal *vhd =
			(struct per_vhost_data__minimal *)d;
	struct msg amsg;
	int len = 128, index = 1, n;

	do {
		/* don't generate output if client not connected */
		if (!vhd->established)
			goto wait;

		pthread_mutex_lock(&vhd->lock_ring); /* --------- ring lock { */

		/* only create if space in ringbuffer */
		n = (int)lws_ring_get_count_free_elements(vhd->ring);
		if (!n) {
			lwsl_user("dropping!\n");
			goto wait_unlock;
		}

		amsg.payload = malloc(LWS_PRE + len);
		if (!amsg.payload) {
			lwsl_user("OOM: dropping\n");
			goto wait_unlock;
		}
		n = lws_snprintf((char *)amsg.payload + LWS_PRE, len,
			         "tid: %p, msg: %d",
			         (void *)pthread_self(), index++);
		amsg.len = n;
		n = lws_ring_insert(vhd->ring, &amsg, 1);
		if (n != 1) {
			__minimal_destroy_message(&amsg);
			lwsl_user("dropping!\n");
		} else
			/*
			 * This will cause a LWS_CALLBACK_EVENT_WAIT_CANCELLED
			 * in the lws service thread context.
			 */
			lws_cancel_service(vhd->context);

wait_unlock:
		pthread_mutex_unlock(&vhd->lock_ring); /* } ring lock ------- */

wait:
		usleep(100000);

	} while (!vhd->finished);

	lwsl_notice("thread_spam %p exiting\n", (void *)pthread_self());

	pthread_exit(NULL);
}

/*static int
connect_client(struct per_vhost_data__minimal *vhd)
{
	vhd->i.context = vhd->context;
	vhd->i.port = 80;
	vhd->i.address = "192.168.0.11";
	vhd->i.path = "/tokens";
	vhd->i.host = vhd->i.address;
	vhd->i.origin = vhd->i.address;
	vhd->i.ssl_connection = 0;

	vhd->i.protocol = "lws-minimal-broker";
	vhd->i.pwsi = &vhd->client_wsi;

	return !lws_client_connect_via_info(&vhd->i);
}*/

/* ----end of test code ----- */

#define TEST_PERIOD_US 50000

struct pss__client_test {
	int number;
};

struct vhd__client_test {
	const unsigned int *options;
};

static int
callback_client_test(struct lws *wsi, enum lws_callback_reasons reason,
			void *user, void *in, size_t len)
{
	struct pss__client_test *pss = (struct pss__client_test *)user;
	struct vhd__client_test *vhd =
				(struct vhd__client_test *)
				lws_protocol_vh_priv_get(lws_get_vhost(wsi),
						lws_get_protocol(wsi));
	uint8_t buf[LWS_PRE + 20], *p = &buf[LWS_PRE];
	const struct lws_protocol_vhost_options *opt;
	//int n, m;

	/* --- start test code --- */

	const struct msg *pmsg;
	void *retval;
	int n, m, r = 0;

	/* --- end test code --- */
	switch (reason) {
	case LWS_CALLBACK_PROTOCOL_INIT:
	  lwsl_notice("test protocol initiated\n");
		/*vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),
			lws_get_protocol(wsi),
			sizeof(struct vhd__client_test));
		if (!vhd)
			return -1;
		if ((opt = lws_pvo_search(
				(const struct lws_protocol_vhost_options *)in,
				"options")))
			vhd->options = (unsigned int *)opt->value;*/
		break;

	case LWS_CALLBACK_ESTABLISHED:
	  lwsl_notice("test protocol LWS_CALLBACK_ESTABLISHED\n");
		pss->number = 0;
		if (!vhd->options || !((*vhd->options) & 1))
			lws_set_timer_usecs(wsi, TEST_PERIOD_US);
		break;

	case LWS_CALLBACK_SERVER_WRITEABLE:
	  lwsl_notice("test protocol LWS_CALLBACK_SERVER_WRITEABLE\n");
		n = lws_snprintf((char *)p, sizeof(buf) - LWS_PRE, "%d",
				 pss->number++);
		m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
		if (m < n) {
			lwsl_err("ERROR %d writing to di socket\n", n);
			return -1;
		}
		break;

	case LWS_CALLBACK_RECEIVE:
		if (len < 6)
			break;
		if (strncmp((const char *)in, "reset\n", 6) == 0)
			pss->number = 0;
		if (strncmp((const char *)in, "closeme\n", 8) == 0) {
			lwsl_notice("test_inc: closing as requested\n");
			lws_close_reason(wsi, LWS_CLOSE_STATUS_GOINGAWAY,
					 (unsigned char *)"seeya", 5);
			return -1;
		}
		break;

	case LWS_CALLBACK_TIMER:
		if (!vhd->options || !((*vhd->options) & 1)) {
			lws_callback_on_writable_all_protocol_vhost(
				lws_get_vhost(wsi), lws_get_protocol(wsi));
			lws_set_timer_usecs(wsi, TEST_PERIOD_US);
		}
		break;

	default:
		break;
	}

	return 0;
}

#define LWS_PLUGIN_PROTOCOL_CLIENT_TEST \
	{ \
		"test-client-protocol", \
		callback_client_test, \
		sizeof(struct pss__client_test), \
		10, /* rx buf size must be >= permessage-deflate rx size */ \
		0, NULL, 0 \
	}

#if !defined (LWS_PLUGIN_STATIC)

static const struct lws_protocols protocols[] = {
	LWS_PLUGIN_PROTOCOL_CLIENT_TEST
};

LWS_EXTERN LWS_VISIBLE int
init_protocol_client_test(struct lws_context *context,
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
destroy_protocol_client_test(struct lws_context *context)
{
	return 0;
}

#endif
