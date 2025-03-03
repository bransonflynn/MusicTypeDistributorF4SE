#include "Internal/Messaging.hpp"
#include "Internal/Distributor.hpp"
#include "Internal/Parser.hpp"

namespace Internal
{
	void Messaging::Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		logger::debug("Messaging: Received message of type: {}"sv, a_msg->type);

		switch (a_msg->type) {
			case F4SE::MessagingInterface::kGameDataReady: {
				// distribute sounds
				CSimpleIniA ini;
				Parser::ParseINIs(ini);
				Parser::PrepareDistributionMap();
				Distributor::Distribute();
				break;
			}
			default: {
				break;
			}
		}

		logger::debug("Messaging: Finished processing for message of type: {}"sv, a_msg->type);
	}
}
