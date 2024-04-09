#include <QString>
#include <iostream>
#include <QWidget>
#include <stdlib.h>
#include <QByteArray>
#include <boost/log/trivial.hpp>
#include "app/arbiter.hpp"
#include "app/widgets/vehicle.hpp"
#include "openauto/Service/InputService.hpp"
#include "AAHandler.hpp"
#include "plugins/vehicle_plugin.hpp"

#define S80_LOG(severity) BOOST_LOG_TRIVIAL(severity) << "[S80VehiclePlugin] "

class ActionsWindow : public QWidget {
    Q_OBJECT
    public:
        ActionsWindow(Arbiter &arbiter, QWidget *parent = nullptr);
};
class InfoWindow : public QWidget {
    Q_OBJECT
    public:
        InfoWindow(Arbiter &arbiter, QWidget *parent = nullptr);
        QLabel* battStatus;
        QLabel* fuelStatus;
        QLabel* cabinTempStatus;
        QLabel* speedStatus;
        QLabel* rpmStatus;
        QLabel* reverseStatus;
};
class VolvoS80 : public QObject, VehiclePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VehiclePlugin_iid)
    Q_INTERFACES(VehiclePlugin)
    public:
        VolvoS80() {};
        ~VolvoS80();
        bool init(ICANBus* canbus) override;
        QList<QWidget *> widgets() override;
        int REVERSE = 0;
        int RPM = 0;
        int SPEED = 0;
        void SendDiagnosticRequest();
    private:
        int REVERSE_TIMEOUT = 0;
        bool inReverse = false;
        ICANBus* canbus;
        void controlls(QByteArray payload);
        void MonitorReverse(QByteArray payload);
        void MonitorRPM(QByteArray payload);
        void diagResponse(QByteArray payload);
        void OpenTrunk();
        void OpenWindows();
        void CloseWindows();
        void GaugeSweep();
        AAHandler *aa_handler;
        Vehicle *vehicle;
        ActionsWindow *actions;
        InfoWindow *info;
};