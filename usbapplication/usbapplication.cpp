

#include <iostream>
#include <libusb.h>
#pragma warning(disable:4996)

constexpr auto CACHE_MAX = 32;


typedef struct usb_device {

	uint8_t address_field;
	unsigned char flag;
	libusb_device* device_ptr;
	short vendor_id;
	short device_id;
	unsigned char manufactorer_string[200];
};


int current_cache_size = 0;
libusb_device** list;
libusb_device* device;
usb_device* device_list[CACHE_MAX] = { NULL };
const libusb_version* ptr;



void new_device(libusb_device** list, libusb_device* device);
void create_device_decision(libusb_device** list, libusb_device* device);
void init();
void add_to_cache(libusb_device* device, uint8_t device_address, usb_device* usb_device_ptr, libusb_device_descriptor device_descriptor, unsigned char manufactorer_string[200]);
void write_out_device_addresses();
int check_for_duplicate_devices(libusb_device_descriptor device_descriptor);








int main() {

	init();
	create_device_decision(list, device);
	return 0;
}






void init() {

	printf("Starting up program....\n");




	if (libusb_init(NULL) != 0) {

		printf("There has been an error...\n");
		libusb_exit(NULL);
		exit(0);


	}
	else {

		printf("LIBUSB: OK\n");
		ptr = libusb_get_version();
		printf("LIBUSB Version: %u\n", ptr->major);

		if (libusb_has_capability != 0) {
		
			printf("LIBUSB Hotplug capability: OK\n");

		}
		else {

			printf("LIBUSB Hotplug capability: NOT OK\n");
		}

		printf("DONE\n");
	}

	






}

void create_device_decision(libusb_device** list, libusb_device* device) {
	 
	printf("\t\tPick an option\n |1. create a new device profile            |\n |2. Use a existing device that is connected|\n |3. Change program behavior                |\n |4. Exit program                           |\n");

	int in;

	while (std::cin >> in) {

		switch (in) {
		

			case 1:
				new_device(list, device);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			default:
				exit(0);
				break;
		}


	}





}



void new_device(libusb_device** list, libusb_device* device) {

	ssize_t devices_i = libusb_get_device_list(NULL, &list);
	ssize_t i = 0;

	for (i = 0; i < devices_i; i++) {
		uint8_t current_device_address;

		if (list[i] != NULL) {
		
		libusb_device* device_iter = list[i];
		libusb_device_handle* device_handle;
		libusb_device_descriptor device_descriptor = { NULL };
		libusb_open(device_iter, &device_handle);
		libusb_get_device_descriptor(device_iter, &device_descriptor);
		current_device_address = libusb_get_device_address(device_iter);

		if (check_for_duplicate_devices(device_descriptor) == 1) {

		
		unsigned char manufacturer_string[200];
		libusb_get_string_descriptor_ascii(device_handle, device_descriptor.iManufacturer, manufacturer_string, 200);
		device_list[current_cache_size] = (usb_device*)malloc(sizeof(usb_device));
		add_to_cache(device_iter, current_device_address, device_list[current_cache_size], device_descriptor, manufacturer_string);
		current_cache_size++;

		}

		}
	}

	printf("Done!\n------------------------------------------------\n");
	write_out_device_addresses();
}


void add_to_cache(libusb_device* device, uint8_t device_address, usb_device* usb_device_ptr, libusb_device_descriptor device_descriptor, unsigned char manufactorer_string[200]) {

	usb_device_ptr->address_field = device_address;
	usb_device_ptr->device_ptr = device;
	usb_device_ptr->flag = 0;
	usb_device_ptr->vendor_id = device_descriptor.idVendor;
	usb_device_ptr->device_id = device_descriptor.idProduct;
	strcpy((char*)usb_device_ptr->manufactorer_string, (char*) manufactorer_string);

		



}


int check_for_duplicate_devices(libusb_device_descriptor device_descriptor) {



	for (int i = 0; i < current_cache_size; i++) {

		if (device_list[i]->vendor_id == device_descriptor.idVendor && device_list[i]->device_id == device_descriptor.idProduct) {

			return 0;
		}


	}
	return 1;
}

void write_out_device_addresses() {

	for (int i = 0; i < current_cache_size; i++)
	{


		printf("Device has format (Vendor:Product)  =  %04x:%04x\n", device_list[i]->vendor_id, device_list[i]->device_id);
		printf("Device descriptor:%s\n", device_list[i]->manufactorer_string);
		printf("------------------------------------------------\n");

	}

	printf("Refresh display? y/n \n");

	char in;

	while (std::cin >> in) {

		if (in == 'y') {
		
			new_device(list, device);
			
		}
		else {

			exit(1);
		}
	}
}

