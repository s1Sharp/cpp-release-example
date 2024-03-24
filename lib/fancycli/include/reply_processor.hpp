#ifndef REPLY_PROCESSOR_HPP
#define REPLY_PROCESSOR_HPP

#include <iostream>
#include <vector>
#include <boost/signals2.hpp>

class Observable {
public:
    Observable() = default;

    void addObserver(const boost::signals2::connection& connection) {
        connections_.push_back(connection);
    }

    void notifyObservers(int arg) {
        for (auto& connection : connections_) {
            // connection(arg);
        }
    }

private:
    std::vector<boost::signals2::connection> connections_;
};

class Observer {
public:
    Observer(Observable& observable) {
        connection_ = observableSignal_.connect([this](int arg) {
            this->handleNotification(arg);
        });
        observable.addObserver(connection_);
    }

    void handleNotification(int arg) {
        std::cout << "Observer received notification with argument: " << arg << std::endl;
    }

private:
    boost::signals2::scoped_connection connection_;
    boost::signals2::signal<void(int)> observableSignal_;
};

// int main() {
//     Observable observable;
//     Observer observer(observable);

//     std::cout << "Notifying observers with argument 42..." << std::endl;
//     observable.notifyObservers(42);

//     return 0;
// }

class IReplyProcessor {
	
};

#endif
