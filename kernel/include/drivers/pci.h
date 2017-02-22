#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std.h>

#include <cpu/io.h>

#define PCI_VENDOR_ID            0x00 // 2
#define PCI_DEVICE_ID            0x02 // 2
#define PCI_COMMAND              0x04 // 2
#define PCI_STATUS               0x06 // 2
#define PCI_REVISION_ID          0x08 // 1

#define PCI_ADDRESS_PORT 0xCF8
#define PCI_DATA_PORT 0xCFC

class PCI_Device {
    uint32_t device;

public:
    uint8_t bus;
    uint8_t slot;
    uint8_t funct;

    uint32_t address;

    char* baseDesc;
    char* subDesc;

    PCI_Device() { }
    ~PCI_Device() { }

    PCI_Device(uint32_t device) {
        this->device = device;

        this->bus = (uint8_t)(device >> 16);
        this->slot = (uint8_t)(device >> 8);
        this->funct = (uint8_t)(device);

        this->baseDesc = "";
        this->subDesc = "";
    }

    PCI_Device(uint8_t bus, uint8_t slot, uint8_t funct) {
        this->device = (bus << 16) | (slot << 8) | funct;

        this->address = (0x80000000 | ((bus) << 16) | ((slot) << 11) | ((funct) << 8));

        this->bus = bus;
        this->slot = slot;
        this->funct = funct;

        this->baseDesc = "";
        this->subDesc = "";
    }

    void setDescriptives(char*  baseDesc, char*  subclassDesc);
    
    // uint32_t readField(size_t field, size_t length);
    // void writeField(size_t field, size_t length, uint32_t value);
};

// class PCI_Controller {
    
// public:
//     PCI_Controller() {
//         this->devices = std::vector<PCI_Device>();
//     }

//     void Initialise();
// };

class PCI {
public:
    static void Initialise();

    static uint16_t readConfig_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset);

    static uint16_t getVendor(uint8_t bus, uint8_t slot, uint8_t function);
    static string getVendorName(uint16_t vendorId);

    static uint16_t getDevice(uint8_t bus, uint8_t slot, uint8_t function);

    static uint8_t getClassId(uint8_t bus, uint8_t slot, uint8_t function);
    static uint8_t getSubclassId(uint8_t bus, uint8_t slot, uint8_t function);

    static string getDeviceType(uint8_t bus, uint8_t slot, uint8_t function);

    //static string getDeviceDesc(size_t vendorId, size_t deviceId);
};

extern std::vector_debug<PCI_Device> pci_devices;

void init_pci();