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

#define DUMB_PERIOD_US 500 * 1000
char token_req_str[512];
int token_count = 0;
char dumb_count_str[10];


struct pss__token {
	int number;
	bool token_received;
	char token_message[512];
};

struct vhd__token {
	const unsigned int *options;
};

int char_count(char ch, char* str) {
	int count = 0;
	/*char *ret;

	ret = strchr(str, ch);
	while (true) {
		if (!ret) break;
		count++;
		ret = strchr(&ret, ch);
		printf("char_count %d in %s\n",count, ret);
	}*/
	for (count=0; str[count]; str[count]==ch ? count++ : *str++);
	//printf("char_count %d in %s\n",count, str);
	return count;
}

void store_char(char * key, char * value)
{
    char tag[50] = "[store_char]";
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        const esp_partition_t* nvs_partition = esp_partition_find_first(
                ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
        assert(nvs_partition && "partition table must have an NVS partition");
        ESP_ERROR_CHECK( esp_partition_erase_range(nvs_partition, 0, nvs_partition->size) );
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle\n", err);
    } else {

	size_t size;
        err = nvs_get_str(my_handle, key, NULL, &size);
	char* previous_value = malloc(size);
	nvs_get_str(my_handle, key, previous_value, &size);
        switch (err) {
            case ESP_OK:
                //printf(tag,"Done\n");
                //printf("%s current value: %s\n", tag, previous_value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("%s value not initialized %s\n",tag, key);
                break;
            default :
                printf("%s Error (%d) reading %s\n", tag, err, key);
        }

        err = nvs_set_str(my_handle, key, value);

        if (err == ESP_OK) {
	  //printf("%s:%s\n",key,value);
	}
	else {
	  printf("%s write to flash failed\n",tag);
	}

        err = nvs_commit(my_handle);
        if (err == ESP_OK) {
	  printf("%s %s:%s\n",tag,key,value);
	}
	else {
	  printf("%s commiting to flash failed!\n", tag);
	}

        // Close
        nvs_close(my_handle);
    }
}

void store_u32(char * key, uint32_t value)
{
    char tag[50] = "[store_u32]";
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        const esp_partition_t* nvs_partition = esp_partition_find_first(
                ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
        assert(nvs_partition && "partition table must have an NVS partition");
        ESP_ERROR_CHECK( esp_partition_erase_range(nvs_partition, 0, nvs_partition->size) );
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle\n", err);
    } else {

        err = nvs_set_u32(my_handle, key, value);

        if (err == ESP_OK) {
	  printf("%s %s:%u\n", tag, key, value);
	}
	else {
	  printf("%s write failed %u\n", tag, value);
	  //printf("%s write to flash failed\n",tag);
	}

        err = nvs_commit(my_handle);
        if (err == ESP_OK) {
	  //printf("%s committed %s:%u\n", tag, key, value);
	  //printf("%s stored %s:%d\n",tag,key,*value);
	}
	else {
	  printf("%s commiting to flash failed!\n", tag);
	}

        // Close
        nvs_close(my_handle);
    }
}

uint32_t get_u32(char * key, uint32_t value)
{
    char tag[50] = "[get_u32]";
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        const esp_partition_t* nvs_partition = esp_partition_find_first(
                ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, NULL);
        assert(nvs_partition && "partition table must have an NVS partition");
        ESP_ERROR_CHECK( esp_partition_erase_range(nvs_partition, 0, nvs_partition->size) );
        // Retry nvs_flash_init
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle\n", err);
    } else {
	err = nvs_get_u32(my_handle, key, &value);
        switch (err) {
            case ESP_OK:
                printf("%s %s:%u\n", tag, key, value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("%s %s not initialized\n",tag, key);
                break;
            default :
                printf("%s Error (%d) reading %s\n", tag, err, key);
        }
        nvs_close(my_handle);
    }
    return value;
}


int
add_headers(void *in, size_t len) {
	char **h = (char **)in;

	if (len < 100)
		return 1;

	*h += sprintf(*h, "x-device-id: %s\x0d\x0a",device_id);
	*h += sprintf(*h, "x-device-token: %s\x0d\x0a",token);

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
		wsi_connect = false;
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
		add_headers(in,len);
		/*printf("\ndevice_id: %s\ntoken: %s\n\n",device_id,token);
		char **h = (char **)in;
		*h += sprintf(*h, "x-device-id: %s\x0d\x0a",device_id);
		*h += sprintf(*h, "x-device-token: %s\x0d\x0a",token);*/
		break;

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		lws_callback_on_writable(wsi);
		//lws_callback_on_writable_all_protocol_vhost(
		//	lws_get_vhost(wsi), lws_get_protocol(wsi));
		wsi_connect = false;
		pss->number = 0;
		pss->token_received = 0;
		if (!vhd->options || !((*vhd->options) & 1))
			lws_set_timer_usecs(wsi, DUMB_PERIOD_US);
		break;

		case LWS_CALLBACK_CLIENT_WRITEABLE:
			//printf("LWS_CALLBACK_CLIENT_WRITEABLE\n");
			//break;
			//if (token_received) break;
			strcpy(token_req_str, "{\"event_type\":\"getUUID\"}");

			n = lws_snprintf((char *)p, sizeof(token_req_str) - LWS_PRE, "%s", token_req_str);
			m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
			if (m < n)
				lwsl_err("ERROR %d writing to token socket\n", n);
			else  {
				//printf("%s\n",token_req_str);
			}
			break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
	  //printf("%s\n",(const char *)in);
		if (pss->token_received) break;
		if (token_count > 4) {
			strcpy(pss->token_message,"");
			token_count = 0;
		} else token_count++;

		strcat(pss->token_message,(const char *)in);
		if (strchr(pss->token_message, '}')) {
			printf("token_message: %s\n",pss->token_message);
			cJSON *root = cJSON_Parse(pss->token_message);
			if (cJSON_GetObjectItem(root,"token")) {
				sprintf(token,"%s",cJSON_GetObjectItem(root,"token")->valuestring);
				printf("received token: %s\n", token);
				pss->token_received = true;
				wsi_token = NULL;
				//wsi_connect = true;
				//store_char("token",token);
			}
			strcpy(pss->token_message,"");
			token_count = 0;
		}
		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		//wsi_connect = true;
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
