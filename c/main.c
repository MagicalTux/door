#include <stdio.h>
#include <ftdi.h>

int main(int argc, char *argv[]) {
	int ret;
	struct ftdi_context ftdic;
	unsigned char buf[1];

	if (ftdi_init(&ftdic) < 0) {
		fprintf(stderr, "ftdi_init failed\n");
		return EXIT_FAILURE;
	}
	if ((ret = ftdi_usb_open(&ftdic, 0x0403, 0x6001)) < 0) {
		fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
		return EXIT_FAILURE;
	}

	// TODO
	ftdi_set_baudrate(&ftdic, 921600);
	if ((ret = ftdi_set_bitmode(&ftdic, 0xff, BITMODE_BITBANG)) < 0) {
		fprintf(stderr, "unable to enable bitbang mode: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
		return EXIT_FAILURE;
	}

	buf[0] = 0xc; // OPEN
	if (argc == 2) {
		switch(argv[1][0]) {
			case 'o': buf[0] = 0xc; break;
			case 'c': buf[0] = 0x3; break;
		}
	}
	ftdi_write_data(&ftdic, buf, 1);
	usleep(1000000);
	buf[0] = 0;
	ftdi_write_data(&ftdic, buf, 1);

	if ((ret = ftdi_usb_close(&ftdic)) < 0) {
		fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
		return EXIT_FAILURE;
	}

	ftdi_deinit(&ftdic);
	return EXIT_SUCCESS;
}


