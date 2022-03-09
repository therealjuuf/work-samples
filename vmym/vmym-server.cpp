#include "Hook.h"
#include "PacketStructures.h"
#include "vmym.h"
#include "PSGameFuncs.h"
#include <random>
#include <algorithm>

vmym_main* VMYM_main; 

void _stdcall VMYM_BoxOpen(int user, int packet) {
	vmym_openbox_packet_recv* recv = (vmym_openbox_packet_recv*)packet;
	vmym_openbox_packet_recv response2;
	response2.BoxID = recv->BoxID;
  
	_SendZonePacket(user, &response2, sizeof(vmym_openbox_packet_recv));
  
	vmym_main_packet_response response;
	auto player = (CUser*)user;

	VMYM_main->boxes[recv->BoxID].State = 0;

	for (int i = 0; i < 26; i++) {
		response.boxes[i].rewardID = VMYM_main->boxes[i].rewardID;
		response.boxes[i].State = VMYM_main->boxes[i].State;
	}
	_SendZonePacket(user, &response, sizeof(vmym_main_packet_response));

}


void _stdcall VMYM_Initialize(int user, int packet) {
	
	vmym_main_packet_response response;
	auto player = (CUser*)user;

	VMYM_main = new vmym_main();

	std::vector<int> RewardIDs;
	for (int i = 0; i < 26; i++) {
		RewardIDs.push_back(i);
	}

	for (int i = 0; i < 26; i++) {

		std::random_device rd;
		auto rng= std::default_random_engine(rd());

		std::shuffle(std::begin(RewardIDs), std::end(RewardIDs), rng);
		VMYM_main->boxes[i].rewardID = RewardIDs.back();
		VMYM_main->boxes[i].State = 1;
		response.boxes[i].rewardID = VMYM_main->boxes[i].rewardID;
		response.boxes[i].State = VMYM_main->boxes[i].State;
		RewardIDs.pop_back();
	}

	_SendZonePacket(user, &response, sizeof(vmym_main_packet_response));
}


void _stdcall VMYM_PacketHandler(int user, int packet) {

	auto player = (CUser*)user;
	if (player->adminStatus != 1) return;

	vmym_main_packet_recv* received = (vmym_main_packet_recv*)packet;
	switch (received->PacketType) {
		case 0: {
			VMYM_Initialize(user, packet);
			break;
		}
		case 1: {
			VMYM_BoxOpen(user, packet);
			break;
		}
	}


}
