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
    private:
        void OpenTrunk();
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
    private:
        void controlls(QByteArray payload);
        AAHandler *aa_handler;
        Vehicle *vehicle;
        ActionsWindow *actions;
};
