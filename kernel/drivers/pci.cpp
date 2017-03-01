#include <stddef.h>
#include <stdint.h>

#include <std.h>
#include <tty.h>
#include <cpu/io.h>
#include <drivers/pci.h>
#include <devices/pci-devices.h> // list of PCI devices
#include <devices/ide.h>

std::vector<PCI_Device> pci_devices = std::vector<PCI_Device>();

void PCI_Device::setDescriptives(char* baseDesc, char*  subclassDesc) {
    this->baseDesc = baseDesc;
    this->subDesc = subclassDesc;
}

uint16_t PCI::readConfig_word(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint32_t addr;
	uint32_t bus32 = bus;
	uint32_t slot32 = slot;
	uint32_t func32 = function;

	addr = (uint32_t)((bus32 << 16) | (slot32 << 11) |
           (func32 << 8) | (offset & 0xfc) | ((uint32_t)0x80000000)); 

	outportl(PCI_ADDRESS_PORT, addr);

	return (uint16_t)((inportl(PCI_DATA_PORT) >> ((offset & 2) * 8)) & 0xffff); 
}

uint16_t PCI::getVendor(uint8_t bus, uint8_t slot, uint8_t function) {
    return PCI::readConfig_word(bus, slot, function, 0);
}

uint16_t PCI::getDevice(uint8_t bus, uint8_t slot, uint8_t function) {
    return PCI::readConfig_word(bus, slot, function, 2);
}


uint8_t PCI::getClassId(uint8_t bus, uint8_t slot, uint8_t function) {
    return (uint8_t)(PCI::readConfig_word(bus, slot, function, 10) >> 8); // this short contains 8 bits of CLID, and 8 bits of SUBCLID
}

uint8_t PCI::getSubclassId(uint8_t bus, uint8_t slot, uint8_t function) {
    return (uint8_t)(PCI::readConfig_word(bus, slot, function, 10)); // this short contains 8 bits of CLID, and 8 bits of SUBCLID
}

// only call this on boot! very resource intensive
void PCI::Initialise() {
    for(unsigned bus=0; bus<256; bus++) {
		for(unsigned slot=0; slot<32; slot++) {
			for(unsigned func=0; func<8; func++) {
                uint16_t vendor = PCI::getVendor(bus, slot, func);
                uint16_t device = PCI::getDevice(bus, slot, func);

                uint8_t classId = PCI::getClassId(bus,slot,func);
                uint8_t subclassId = PCI::getSubclassId(bus,slot,func);

                if(vendor != 0xFFFF && classId != 0xFF) {
                    // we have a connected device!
                    //pci_devices.push_back(bus);

                    PCI_Device device(bus, slot, func);

                    for(size_t i = 0; i < PCI_DEVTABLE_LEN; i++) {
                        if(PciClassCodeTable[i].BaseClass == classId && 
                           PciClassCodeTable[i].SubClass == subclassId &&
                           PciClassCodeTable[i].ProgIf == 0x00 ) 
                        {
                            device.setDescriptives((char*)PciClassCodeTable[i].BaseDesc, (char*)PciClassCodeTable[i].SubDesc);
                            break;
                        }
                    }

                    if(device.baseDesc[0] == '\0') {
                        terminal_printf("[PCI] %x (%x)  (%d:%d.%d)\n", classId, subclassId, bus, slot, func);
                    } else {
                        terminal_printf("[PCI] %s (%s) %x (%d:%d.%d)\n", device.baseDesc, device.subDesc, device.address, bus, slot, func);
                    }
                }
            }
        }
    }
}

void init_pci() {
    // PCI_Controller pcidriver;
	// pcidriver.Initialise();

    PCI::Initialise();

    //pci_devices.push_back(PCI_Device());
    // std::vector<int> arr = std::vector<int>();
    // terminal_writestring("got here 1");
    // arr.push_back(1);
    // terminal_writestring("got here 2");
    // arr.push_back(2);
}