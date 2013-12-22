#ifdef SHOCKER_UNIT_TEST
#include "shocker.h"
#include <iostream>
#include <thread>
#define TEST
using namespace shocker;

int main(int argc, char* argv[])
{
	std::map<std::string, std::string> args;
#ifdef TEST
	args["sound_filename"] = "sound.wav";
	args["movie_filename"] = "short.mpg";
	args["finger_start_moving_time"] = "2.0";//sec
	args["tms_shock_rational_time"] = "0.1";//sec(= after 100ms)
	args["serial_com_id"] = "/dev/ttyUSB0";
	args["number_of_trials"] = "240";
#else
	if(argc != 7){
		std::cout << "usage:" << std::endl;
		return 0;
	}
	args["sound_filename"] = argv[1];
	args["movie_filename"] = argv[2];
	args["finger_start_moving_time"] = argv[3];
	args["tms_shock_ratinal_time"] = argv[4];
	args["serial_com_id"] = argv[5];
	args["number_of_trials"] = argv[6];
#endif
	boost::asio::io_service io_service;
	shocker::tms_shocker s(io_service, 
			args["sound_filename"], args["movie_filename"], 
			std::stod(args["finger_start_moving_time"]), 
			std::stod(args["tms_shock_rational_time"]),
			args["serial_com_id"],
			std::stoi(args["number_of_trials"]));
	boost::asio::io_service::work work(io_service);
	std::thread t1([&io_service](){ io_service.run(); });
	std::thread t2([&io_service](){ io_service.run(); });
	io_service.post([&s](){ s.run(); });
	io_service.run();
	t1.join();
	t2.join();

    return 0;
}

#endif
