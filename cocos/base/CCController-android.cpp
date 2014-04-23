#include "CCController.h"
#include "CCGamepad.h"
#include "CCControllerDirectionPad.h"
#include "CCControllerButtonInput.h"
#include "CCControllerAxisInput.h"
#include "CCControllerThumbstick.h"

#include "ccMacros.h"
#include "CCEventDispatcher.h"
#include "CCEventController.h"
#include "CCEventListenerController.h"
#include "CCDirector.h"

const std::vector<Controller*>& Controller::getControllers()
{
}

void Controller::startDiscoveryController()
{

}

void Controller::stopDiscoveryController()
{
}

const std::string& Controller::getVendorName()
{

}

bool Controller::isConnected() const
{
}

int Controller::getPlayerIndex() const
{
}

void Controller::setPlayerIndex(int playerIndex)
{
}

Gamepad* Controller::getGamepad()
{
}

Controller::Controller()
{

}

Controller::~Controller()
{

}
