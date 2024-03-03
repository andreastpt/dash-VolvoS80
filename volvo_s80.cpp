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

        if(DEBUG)
            this->debug = new DebugWindow(*this->arbiter);

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
    if(payload.at(1) & 0b1000 != 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::UP);
    }
    if(payload.at(1) & 0b0100 != 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::DOWN);
    }
    if(payload.at(1) & 0b0001 != 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::LEFT);
    }
    if(payload.at(1) & 0b0010 != 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::RIGHT);
    }
    if(payload.at(1) & 0b10000 != 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::BACK);
    }
    if(payload.at(1) & 0b100000 != 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::ENTER);
    }
    if(payload.at(0) & 0b10000 == 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::TOGGLE_PLAY);
    }
    if(payload.at(0 )& 0b100000 == 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::CALL_END);
    }
    if(payload.at(0) & 0b0010 == 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::NEXT);
    }
    if(payload.at(0) & 0b0001 == 0){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::PREV);
    }
}
bool oldStatus = true;