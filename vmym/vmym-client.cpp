#include "asm.h"
#include "vmym.h"
#include "Definitions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//_vmym_main* VMYM;
_vmym_main* VMYM = new _vmym_main();

void VMYM_InitializeRewards() {
	std::ifstream file("Inc/vmym/rewards.txt");
	std::string str;
	int i = 0;
	while (std::getline(file, str)) {
		//MapNames2[i] = str;
		VMYM->Rewards[i].RewardName = str;
		i++;
	}
}


void _stdcall VMYM_BoxOpen_recv(int packet) {
	vmym_openbox_packet_recv* recv = (vmym_openbox_packet_recv*)packet;
	VMYM->activebox = new vmym_box_active();
	VMYM->activebox->boxid = recv->BoxID;
	VMYM->activebox->RewardID = VMYM->boxes[recv->BoxID].rewardID;
}

void _stdcall VMYM_Main_recv(int packet) {
	vmym_main_packet_response* recv = (vmym_main_packet_response*)packet;

	for (int i = 0; i < 26; i++) {
		VMYM->boxes[i].rewardID = recv->boxes[i].rewardID;
		VMYM->boxes[i].State = recv->boxes[i].State;
		VMYM->Rewards[VMYM->boxes[i].rewardID].State = recv->boxes[i].State;
	}

}

void _stdcall VMYM_Main_send() {
	vmym_main_packet_recv response;
	SendPacket(&response, sizeof(vmym_main_packet_recv));
}

void _stdcall VMYM_OpenBox_send(int OpenedBox) {
	vmym_openbox_packet_recv response;
	response.BoxID = OpenedBox;
	SendPacket(&response, sizeof(vmym_openbox_packet_recv));
}


void _stdcall VMYM_PacketHandler(int packet) {

	vmym_main_packet_recv* received = (vmym_main_packet_recv*)packet;
	switch (received->PacketType) {
    case 0: {
      VMYM_Main_recv(packet);
      break;
    }
    case 1: {
      VMYM_BoxOpen_recv(packet);
      break;
    }
	}

}


void VMYM_main() {
	VMYM_InitializeRewards();
}
