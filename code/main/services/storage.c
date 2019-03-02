#include "nvs_flash.h"
#include "nvs.h"
char tag[50] = "[storage.c]";
bool storage_in_use = false;

void storage_init () {
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
}

char * get_char(char * key)
{
    // esp_intr_noniram_disable();

    nvs_handle my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (err != ESP_OK) {
      printf("Error (%d) opening NVS handle\n", err);
    } else {
      size_t required_size;
      err = nvs_get_str(my_handle, key, NULL, &required_size);
      char* value_str = malloc(required_size);
      nvs_get_str(my_handle, key, value_str, &required_size);

      switch (err) {
        case ESP_OK:
          //printf(tag,"Done\n");
          //printf("%s current value: %s\n", tag, previous_value);
          break;
        case ESP_ERR_NVS_NOT_FOUND:
          printf("%s value not initialized, key: %s\n",tag, key);
          return "";
        default :
          printf("%s Error (%d) reading %s\n", tag, err, key);
      }

      //free(value_str);
      nvs_close(my_handle);
      //printf("key: %s, value: %s\n",key,value_str);
      // esp_intr_noniram_enable();
      return value_str;
    }
    // esp_intr_noniram_enable();
    return ""; //return "" if no token found
}

void store_char(char * key, char * value) {
  storage_in_use = true;
  nvs_handle my_handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK) {
      printf("Error (%d) opening NVS handle\n", err);
  } else {

    err = nvs_set_str(my_handle, key, value);
    if (err == ESP_OK) {
	     printf("%s nvs_set_str for %s\n", tag, key);
	  } else printf("%s write to flash failed\n", tag);

    err = nvs_commit(my_handle);
    if (err == ESP_OK) {
	     printf("%s nvs_commit %s:%s\n", tag, key, value);
	  } else printf("%s commiting to flash failed!\n", tag);

    nvs_close(my_handle);
  }
  storage_in_use = false;
}

void store_char_old(char * key, char * value) {
  nvs_handle my_handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
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
      printf("%s Current value: %s\n", tag, previous_value);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      printf("%s value not initialized %s\n",tag, key);
      break;
    default :
      printf("%s Error (%d) reading %s\n", tag, err, key);
    }

    err = nvs_set_str(my_handle, key, value);
    if (err == ESP_OK) {
	     printf("%s nvs_set_str for %s\n", tag, key);
	  } else printf("%s write to flash failed\n", tag);

    esp_intr_noniram_enable();
    err = nvs_commit(my_handle);
    if (err == ESP_OK) {
	     printf("%s nvs_commit %s:%s\n", tag, key, value);
	  } else printf("%s commiting to flash failed!\n", tag);

    nvs_close(my_handle);
  }
}

void store_u32(char * key, uint32_t value) {
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

uint32_t get_u32(char * key, uint32_t value) {
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
