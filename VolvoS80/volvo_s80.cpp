#include "volvo_s80.hpp"
#define DEBUG false
#define PRINT_CAN_PAYLOADS false
#define SHOW_VEHICLE_WIDGET true
#include <chrono>
unsigned long millis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
VolvoS80::~VolvoS80()
{
    if(this->vehicle)
        delete this->vehicle;
    if(this->actions)
        delete this->actions;
    if(this->info)
        delete this->info;
}
bool VolvoS80::init(ICANBus* canbus)
{
    if (this->arbiter) {
        this->aa_handler = this->arbiter->android_auto().handler;
        if (SHOW_VEHICLE_WIDGET){
            this->vehicle = new Vehicle(*this->arbiter);
            this->vehicle->setObjectName("Volvo S80");
            this->vehicle->disable_sensors();
            this->vehicle->rotate(270);
        }
        this->actions = new ActionsWindow(*this->arbiter);
        this->actions->setObjectName("Actions");
        this->info = new InfoWindow(*this->arbiter);
        this->info->setObjectName("Info");
        this->canbus = canbus;
        canbus->registerFrameHandler(0x404066, [this](QByteArray payload){this->controlls(payload);});
        canbus->registerFrameHandler(0x012177FC, [this](QByteArray payload){this->MonitorReverse(payload);});
        canbus->registerFrameHandler(0x02A07428, [this](QByteArray payload){this->MonitorRPM(payload);});
        canbus->registerFrameHandler(0x00800003, [this](QByteArray payload){this->diagResponse(payload);});
        QPushButton* openTrunkButton = this->actions->findChild<QPushButton*>("Open Trunk");
        if (openTrunkButton) {
            connect(openTrunkButton, &QPushButton::clicked, this, [this](){this->OpenTrunk();});
        } 
        QPushButton* openWindowsButton = this->actions->findChild<QPushButton*>("Open Windows");
        if (openWindowsButton) {
            connect(openWindowsButton, &QPushButton::clicked, this, [this](){this->OpenWindows();});
        }
        QPushButton* closeWindowsButton = this->actions->findChild<QPushButton*>("Close Windows");
        if (closeWindowsButton) {
            connect(closeWindowsButton, &QPushButton::clicked, this, [this](){this->CloseWindows();});
        }
        QPushButton* sweepButton = this->actions->findChild<QPushButton*>("Gauge Sweep");
        if (sweepButton) {
            connect(sweepButton, &QPushButton::clicked, this, [this](){this->GaugeSweep();});
        } 
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
    if (SHOW_VEHICLE_WIDGET){
        tabs.append(this->vehicle);
    }
    tabs.append(this->actions);
    tabs.append(this->info);
    return tabs;
}
void VolvoS80::diagResponse(QByteArray payload){
    if (DEBUG == true && PRINT_CAN_PAYLOADS == true) {
        S80_LOG(info) << "Payload received: " << payload.toHex().toStdString();
    }
    if(payload.at(1) == 0x40 && payload.at(4) == 0x02){
        this->info->battStatus->setText(QString::number((payload.at(5) / 0.08)));
    }
}
void VolvoS80::MonitorRPM(QByteArray payload){
    if (DEBUG == true && PRINT_CAN_PAYLOADS == true) {
        S80_LOG(info) << "Payload received RPM: " << payload.toHex().toStdString();
    }
    int A = payload.at(6);
    A &= 0b1111;
    RPM = A << 8 | payload.at(7);
    this->info->rpmStatus->setText(QString::number(RPM));
}
void VolvoS80::controlls(QByteArray payload){
    static qint64 lastButtonPressTime = 0;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (DEBUG == true && PRINT_CAN_PAYLOADS == true) {
        S80_LOG(info) << "Payload received: " << payload.toHex().toStdString();
    }
    if(payload.at(6) == 0x48 && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::UP);
        lastButtonPressTime = currentTime;
	    if (DEBUG) {
		S80_LOG(info) << "UP Pressed";
	    }
    }
    else if(payload.at(6) == 0x44 && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::DOWN);
        lastButtonPressTime = currentTime;
	if (DEBUG) {
		S80_LOG(info) << "DOWN Pressed";
	}
    }
    else if(payload.at(6) == 0x41 && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::LEFT);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "LEFT Pressed";
	}
    }
    else if(payload.at(6) == 0x42 && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::RIGHT);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "RIGHT Pressed";
	}
    }
    else if(payload.at(6) == 0x50 && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::BACK);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "BACK Pressed";
	}
    }
    else if(payload.at(6) == 0x60 && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::ENTER);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "ENTER Pressed";
	}
    }
    else if(payload.at(7) == 0x6F && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::TOGGLE_PLAY);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "PLAY/PAUSE Pressed";
	}
    }
    else if(payload.at(7) == 0x5F && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::CALL_END);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "CALL_END Pressed";
	}
    }
    else if(payload.at(7) == 0x7D && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::NEXT);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "NEXT Pressed";
	}
    }
    else if(payload.at(7) == 0x7E && currentTime - lastButtonPressTime > 300){
        this->aa_handler->injectButtonPress(aasdk::proto::enums::ButtonCode::PREV);
        lastButtonPressTime = currentTime;
        if (DEBUG) {
		S80_LOG(info) << "PREV Pressed";
	}
    SendDiagnosticRequest();
    }
}
ActionsWindow::ActionsWindow(Arbiter &arbiter, QWidget *parent) : QWidget(parent)
{
    QPushButton *openTrunk = new QPushButton("Open Trunk");
    openTrunk->setObjectName("Open Trunk");
    QPushButton *openWindows = new QPushButton("Open Windows");
    openWindows->setObjectName("Open Windows");
    QPushButton *closeWindows = new QPushButton("Close Windows");
    closeWindows->setObjectName("Close Windows");
    QPushButton *sweep = new QPushButton("Gauge Sweep");
    sweep->setObjectName("Gauge Sweep");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(openTrunk);
    layout->addWidget(openWindows);
    layout->addWidget(closeWindows);
    layout->addWidget(sweep);
}
InfoWindow::InfoWindow(Arbiter &arbiter, QWidget *parent) : QWidget(parent)
{
    QLabel* vBatt = new QLabel("Battery Voltage", this);
    QLabel* sFuel = new QLabel("Fuel", this);
    QLabel* tCabin = new QLabel("Cabin Temp", this);
    QLabel* vSpeed = new QLabel("Speed", this);
    QLabel* RPM = new QLabel("RPM", this);
    QLabel* inReverse = new QLabel("In Reverse", this);

    battStatus = new QLabel("--", this);
    fuelStatus = new QLabel("--", this);
    cabinTempStatus = new QLabel("--", this);
    speedStatus = new QLabel("--", this);
    rpmStatus = new QLabel("--", this);
    reverseStatus = new QLabel("--", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(vBatt);
    layout->addWidget(battStatus);
    layout->addWidget(Session::Forge::br(false));
    layout->addWidget(sFuel);
    layout->addWidget(fuelStatus);
    layout->addWidget(Session::Forge::br(false));
    layout->addWidget(tCabin);
    layout->addWidget(cabinTempStatus);
    layout->addWidget(Session::Forge::br(false));
    layout->addWidget(vSpeed);
    layout->addWidget(speedStatus);
    layout->addWidget(Session::Forge::br(false));
    layout->addWidget(RPM);
    layout->addWidget(rpmStatus);
    layout->addWidget(Session::Forge::br(false));
    layout->addWidget(inReverse);
    layout->addWidget(reverseStatus);
    layout->addWidget(Session::Forge::br(false));
}
void VolvoS80::SendDiagnosticRequest()
{
    static qint64 lastRequestTime = 0;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if(currentTime - lastRequestTime > 4500){
        if (DEBUG) {
        S80_LOG(info) << "Sending Diagnostic Request";
        }
        this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CD40A61A02010000")));
        lastRequestTime = currentTime;
    }
}
void VolvoS80::OpenTrunk()
{
    if (DEBUG) {
        S80_LOG(info) << "Opening Trunk";
    }
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CF46B16F51010204")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CD46B16F51000000")));
}
void VolvoS80::OpenWindows()
{
    if (DEBUG) {
        S80_LOG(info) << "Opening Windows";
    }
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("8F40B11A21014001")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("4E00004001000000")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CE43B00901010100")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CD43B01001030002")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CE43B00901010100")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CD43B01001030010")));
}
void VolvoS80::CloseWindows()
{
    if (DEBUG) {
        S80_LOG(info) << "Closing Windows";
    }
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("8F40B11A21018000")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("4E00008000000000")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CE43B00901010100")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CD43B01001030001")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CE43B00901010100")));
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CD43B01001030008")));
}
void VolvoS80::GaugeSweep()
{
    if (DEBUG) {
        S80_LOG(info) << "Gauge Sweep Started";
    }
    this->canbus->writeFrame(QCanBusFrame(0x0FFFFE, QByteArray::fromHex("CB51B20200000000")));

}
void VolvoS80::MonitorReverse(QByteArray payload){
    if (DEBUG == true && PRINT_CAN_PAYLOADS == true) {
        S80_LOG(info) << "Payload received Reverse: " << payload.toHex().toStdString();
    }
    int A = payload.at(6);
    A &= 0b11;
    SPEED = (A << 8 | payload.at(7)) / 4;
    REVERSE = (payload.at(2) >> 5) & 1;
    this->info->speedStatus->setText(QString::number(SPEED));
    if(SHOW_VEHICLE_WIDGET){
        bool rrDoorUpdate = (payload.at(5) == 0x08) & 1;
        bool rlDoorUpdate = (payload.at(5) == 0x04) & 1;
        bool frDoorUpdate = (payload.at(5) == 0x02) & 1;
        bool flDoorUpdate = (payload.at(5) == 0x01) & 1;
        this->vehicle->door(Position::BACK_RIGHT, rrDoorUpdate);
        this->vehicle->door(Position::BACK_LEFT, rlDoorUpdate);
        this->vehicle->door(Position::FRONT_RIGHT, frDoorUpdate);
        this->vehicle->door(Position::FRONT_LEFT, flDoorUpdate);
    }
    if(REVERSE){
	    if(!DEBUG) {
        	this->arbiter->set_curr_page(3);
	    }
        REVERSE_TIMEOUT = millis() + 100;
        this->info->reverseStatus->setText("yes");
        if (DEBUG) {
	        S80_LOG(info) << "in reverse";
	    }
    }
    else{
        if(millis() > REVERSE_TIMEOUT){
            //this->arbiter->set_curr_page(0);
            REVERSE_TIMEOUT = 0;
            this->info->reverseStatus->setText("no");
            if (DEBUG) {
                S80_LOG(info) << "not in reverse";
            }
        }
    }
}
