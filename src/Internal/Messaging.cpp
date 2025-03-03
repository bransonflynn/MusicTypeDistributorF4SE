#include "Internal/Messaging.hpp"

namespace Internal
{
	void Messaging::Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		logger::info("Messaging: Received message of type: {}"sv, a_msg->type);

		switch (a_msg->type) {
			case F4SE::MessagingInterface::kPostLoad: {
				break;
			}
			case F4SE::MessagingInterface::kPostPostLoad: {
				break;
			}
			case F4SE::MessagingInterface::kPreLoadGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostLoadGame: {
				break;
			}
			case F4SE::MessagingInterface::kPreSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kDeleteGame: {
				break;
			}
			case F4SE::MessagingInterface::kInputLoaded: {
				break;
			}
			case F4SE::MessagingInterface::kNewGame: {
				break;
			}
			case F4SE::MessagingInterface::kGameLoaded: {
				break;
			}
			case F4SE::MessagingInterface::kGameDataReady: {
				// distribute sounds
				break;
			}
			default: {
				break;
			}
		}

		logger::info("Messaging: Finished processing for message of type: {}"sv, a_msg->type);
	}
}
