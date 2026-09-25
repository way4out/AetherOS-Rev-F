#include "remote_compute.h"
#include "../network/network_fabric.h"
namespace aether::compute {
void init(){}
bool available(Backend){return network::status(network::LINK_REMOTE_COMPUTE).available;}
bool submit(const Job&j){if(!j.payload)return false;if(!available(j.backend))return false;return true;}
}