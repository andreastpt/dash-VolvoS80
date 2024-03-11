#include "volvo_s80.hpp"
#define DEBUG false
#define PRINT_CAN_PAYLOADS false
VolvoS80::~VolvoS80()
{
    if(this->vehicle)
        delete this->vehicle;
    if(this->actions)
        delete this->actions;
}
bool VolvoS80::init(ICANBus* canbus)
{
    if (this->arbiter) {
        this->aa_handler = this->arbiter->android_auto().handler;
        this->vehicle = new Vehicle(*this->arbiter);
        this->vehicle->setObjectName("Volvo S80");
        this->vehicle->disable_sensors();
        this->vehicle->rotate(270);
        this->actions = new Actions(*this->arbiter);
        this->actions->setObjectName("Actions");
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
    tabs.append(this->actions);
    return tabs;
}
void VolvoS80::controlls(QByteArray payload){
    static qint64 lastButtonPressTime = 0;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (DEBUG == true && PRINT_CAN_PAYLOADS == true) {
        S80_LOG(info) << "Payload received: " << payload.toHex().toStdString();
    }
    if(payload.at(6) == 0x48 && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::UP);
        lastButtonPressTime = currentTime;
	    if (DEBUG) {
		S80_LOG(info) << "UP Pressed";
	    }
    }
    else if(payload.at(6) == 0x44 && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::DOWN);
        lastButtonPressTime = currentTime;
	if (DEBUG) {
		S80_LOG(info) << "DOWN Pressed";
	}
    }
    else if(payload.at(6) == 0x41 && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::LEFT);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "LEFT Pressed";
	}
    }
    else if(payload.at(6) == 0x42 && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::RIGHT);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "RIGHT Pressed";
	}
    }
    else if(payload.at(6) == 0x50 && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::BACK);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "BACK Pressed";
	}
    }
    else if(payload.at(6) == 0x60 && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::ENTER);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "ENTER Pressed";
	}
    }
    else if(payload.at(7) == 0x6F && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::TOGGLE_PLAY);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "PLAY/PAUSE Pressed";
	}
    }
    else if(payload.at(7) == 0x5F && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::CALL_END);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "CALL_END Pressed";
	}
    }
    else if(payload.at(7) == 0x7D && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::NEXT);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "NEXT Pressed";
	}
    }
    else if(payload.at(7) == 0x7E && currentTime - lastButtonPressTime > 100){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::PREV);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "PREV Pressed";
	}
    }
}
ActionsWindow::ActionsWindow(Arbiter &arbiter, QWidget *parent) : QWidget(parent)
{
    QtpushButton *openTrunk = new QPushButton("Open Trunk");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(openTrunk);
    connect(openTrunk, &QPushButton::clicked, this, SLOT(OpenTrunk()));   
}
void ActionsWindow::OpenTrunk()
{
    if (DEBUG) {
        S80_LOG(info) << "Opening Trunk";
    }
    canbus->WriteFrame(QcanbusFrame(0x0FFFFE, QByteArray::fromHex("CF46B16F51010204")));
    canbus->WriteFrame(QcanbusFrame(0x0FFFFE, QByteArray::fromHex("CD46B16F51000000")));
}