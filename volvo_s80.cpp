#include "volvo_s80.hpp"
#define DEBUG false
VolvoS80::~VolvoS80()
{
    if(this->vehicle)
        delete this->vehicle;
}
bool VolvoS80::init(ICANBus* canbus)
{
    if (this->arbiter) {
        this->aa_handler = this->arbiter->android_auto().handler;
        this->vehicle = new Vehicle(*this->arbiter);
        this->vehicle->setObjectName("Volvo S80");
        this->vehicle->disable_sensors();
        this->vehicle->rotate(270);

        canbus->registerFrameHandler(0x404066, [this](QByteArray payload){this->controlls(payload);});
        S80_LOG(info)<<"loaded successfully";
        return true;
    }
    else{
        S80_LOG(error)<<"Failed to get arbiter";
        return false;
    }
}
QList<QWidget *> VolvoS80::widgets()
{
    QList<QWidget *> tabs;
    tabs.append(this->vehicle);
    return tabs;
}
void VolvoS80::controlls(QByteArray payload){
    if (DEBUG) {
        S80_LOG(info) << "Payload received: " << payload.toHex().toStdString();
    }
    if(payload.at(6) == 0x48){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::UP);
	    if (DEBUG) {
		S80_LOG(info) << "UP Pressed";
	    }
    }
    else if(payload.at(6) == 0x44){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::DOWN);
	if (DEBUG) {
		S80_LOG(info) << "DOWN Pressed";
	}
    }
    else if(payload.at(6) == 0x41){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::LEFT);
        if (DEBUG) {
		S80_LOG(info) << "LEFT Pressed";
	}
    }
    else if(payload.at(6) == 0x42){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::RIGHT);
        if (DEBUG) {
		S80_LOG(info) << "RIGHT Pressed";
	}
    }
    else if(payload.at(6) == 0x50){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::BACK);
        if (DEBUG) {
		S80_LOG(info) << "BACK Pressed";
	}
    }
    else if(payload.at(6) == 0x60){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::ENTER);
        if (DEBUG) {
		S80_LOG(info) << "ENTER Pressed";
	}
    }
    else if(payload.at(7) == 0x6F){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::TOGGLE_PLAY);
        if (DEBUG) {
		S80_LOG(info) << "PLAY/PAUSE Pressed";
	}
    }
    else if(payload.at(7) == 0x5F){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::CALL_END);
        if (DEBUG) {
		S80_LOG(info) << "CALL_END Pressed";
	}
    }
    else if(payload.at(7) == 0x7D){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::NEXT);
        if (DEBUG) {
		S80_LOG(info) << "NEXT Pressed";
	}
    }
    else if(payload.at(7) == 0x7E){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::PREV);
        if (DEBUG) {
		S80_LOG(info) << "PREV Pressed";
	}
    }
}