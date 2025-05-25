#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ether.h>
#include <stdint.h>

#define ADC_REF_VOLTAGE 3.3f    // Reference voltage for ADC conversion
#define ADC_RESOLUTION 4096.0f  // 12-bit ADC resolution
#define TOTAL_FLOATS 32  // 128 bytes / 4 bytes per float

float float_array[TOTAL_FLOATS];  // Holds the converted float values

#define INTERFACE "enp1s0"
#define TARGET_MAC "00:80:E1:00:00:00"

void convert_packet_to_floats(const uint8_t *packet, size_t length) ;

void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    printf("\nPacket captured - Length: %d bytes\n", header->len);

    // Print raw packet data in hex format
    for (int i = 0; i < header->len; i++) {
        printf("%02x ", packet[i]);
        if ((i + 1) % 16 == 0) printf("\n"); // Newline for readability
    }
    printf("\n");
    
    // Pass the packet and its length to convert_packet_to_floats()
        convert_packet_to_floats(packet, header->len);
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program fp;
    char filter_exp[64];

    // Define filter to capture packets FROM embedded board MAC
    snprintf(filter_exp, sizeof(filter_exp), "ether src %s", TARGET_MAC);

    // Open network interface for live packet capture
    handle = pcap_open_live(INTERFACE, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        fprintf(stderr, "Could not open device %s: %s\n", INTERFACE, errbuf);
        return 1;
    }

    // Compile and apply filter
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1 ||
        pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Error setting filter: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        return 1;
    }

    printf("Listening for packets from MAC: %s on interface %s...\n", TARGET_MAC, INTERFACE);

    // Capture packets indefinitely and process them using packet_handler
    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_close(handle);
    return 0;
}

void convert_packet_to_floats(const uint8_t *packet, size_t length) {
    if (!packet || length < 128) {
        fprintf(stderr, "Error: Packet size must be at least 128 bytes.\n");
        return;
    }

    // Print raw packet data
    printf("\nRaw Packet Data:\n");
    for (size_t i = 0; i < length; i++) {
        printf("%02x ", packet[i]);
        if ((i + 1) % 16 == 0) printf("\n"); // Formatting for readability
    }
    printf("\n");

    // Convert every 4 bytes into a float and apply voltage scaling
    const uint8_t *src_ptr = packet;
    float *dest_ptr = float_array;

    printf("\nConverted Voltage Values:\n");
    for (int i = 0; i < TOTAL_FLOATS; i++) {
        memcpy(dest_ptr, src_ptr, sizeof(float));  // Copy 4 bytes into float memory
        
        // Apply ADC voltage conversion
        float voltage = (*dest_ptr / ADC_RESOLUTION) * ADC_REF_VOLTAGE;
        printf("Voltage %d: %f V\n", i, voltage);
        
        src_ptr += sizeof(float);
        dest_ptr++;
    }
}

#if 0
void convert_packet_to_floats(const uint8_t *packet, size_t length) {
    if (!packet || length < 128) {
        fprintf(stderr, "Error: Packet size must be at least 128 bytes.\n");
        return;
    }

    // Print raw packet data
    printf("\nRaw Packet Data:\n");
    for (size_t i = 0; i < length; i++) {
        printf("%02x ", packet[i]);
        if ((i + 1) % 16 == 0) printf("\n"); // Formatting for readability
    }
    printf("\n");

    // Convert every 4 bytes into a float
    const uint8_t *src_ptr = packet;
    float *dest_ptr = float_array;

    for (int i = 0; i < TOTAL_FLOATS; i++) {
        memcpy(dest_ptr, src_ptr, sizeof(float));  // Copy 4 bytes into float memory
        src_ptr += sizeof(float);  // Move to next 4-byte block
        dest_ptr++;
    }

    // Print the converted floats
    printf("\nConverted Float Values:\n");
    for (int i = 0; i < TOTAL_FLOATS; i++) {
        printf("Float %d: %f\n", i, float_array[i]);
    }
}

#endif