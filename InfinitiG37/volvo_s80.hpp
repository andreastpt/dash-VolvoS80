#include <QString>
#include <iostream>
#include <stdlib.h>
#include <QByteArray>
#include <boost/log/trivial.hpp>
#include "app/arbiter.hpp"
#include "app/widgets/vehicle.hpp"
#include "openauto/Service/InputService.hpp"
#include "AAHandler.hpp"
#include "plugins/vehicle_plugin.hpp"

#define S80_LOG(severity) BOOST_LOG_TRIVIAL(severity) << "[S80VehiclePlugin] "

class DebugWindow : public QWidget {
    Q_OBJECT

    public:
        DebugWindow(Arbiter &arbiter, QWidget *parent = nullptr);
};
class VolvoS80 : public QObject
{
    Q_OBJECT
    public:
        VolvoS80() {
            this->arbiter = nullptr;
            this->aa_handler = nullptr;
            this->debug = nullptr;
        };
        ~VolvoS80();
        bool init(ICANBus* canbus);
    private:
        void controlls(QByteArray payload);
        AAHandler *aa_handler;
        DebugWindow *debug;
        Arbiter* arbiter;
};