
# **Packet Sniffer**
🚀 **Real-time Ethernet packet capture for analyzing data from EmbeddedEthernet.**

## **Overview**
Packet Sniffer is a **lightweight network monitoring tool** designed to capture and process Ethernet packets from an **embedded system**. Using **libpcap**, it applies filtering to extract packets from a specific MAC address and decodes incoming data into **voltage values**.

✅ Captures raw Ethernet packets  
✅ Filters packets from a defined MAC address  
✅ Converts raw ADC data to voltage readings  
✅ Works alongside **EmbeddedEthernet** for data acquisition  

## **System Requirements**
📌 Linux-based system (tested on Ubuntu)  
📌 **libpcap** installed (`sudo apt install libpcap-dev`)  
📌 STM-based embedded system with Ethernet support  

## **Installation**
Clone the repository and navigate into the directory:
```sh
git clone https://github.com/stevemac321/packet_sniffer.git
cd packet_sniffer
```

## **Usage**
Compile and run:
```sh
gcc -o packet_sniffer packet_sniffer.c -lpcap
sudo ./packet_sniffer
```

📌 **Example Output:**
```
Packet captured - Length: 128 bytes
Converted Voltage Values:
Voltage 0: 1.330151 V
Voltage 1: -0.000000 V
...
```

## **How It Works**
1. **Captures packets from STM-based board** over Ethernet.  
2. **Filters packets** using the specified MAC address.  
3. **Processes ADC data** (128 bytes → 32 floats → converted voltage).  
4. **Displays readable voltage values** in real-time.  

## **Integration with EmbeddedEthernet**
📡 **EmbeddedEthernet** handles **data transmission** from the STM board, sending Ethernet packets with ADC readings.  
🛠 **Packet Sniffer** captures and decodes these packets into voltage values for analysis.  

---
