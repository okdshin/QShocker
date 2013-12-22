#ifndef SHOCKER_SHOCKER_H
#define SHOCKER_SHOCKER_H
//shocker:20131221
#include <iostream>
#include <string>
#include <stdexcept>
#include <functional>
#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

namespace shocker
{
class tms_shocker{
    tms_shocker(const tms_shocker&) = delete;
    auto operator=(const tms_shocker&) -> tms_shocker& = delete;
public:
    tms_shocker(boost::asio::io_service& io_service, std::function<void ()> sound_player,
            const std::string& movie_filename,
			double finger_start_moving_time, double tms_shock_rational_time, 
			const std::string& serial_com_id,
			std::size_t number_of_trials) :
			io_service_(io_service),
            sound_player_(sound_player),
            serial_port_(io_service),
			number_of_trials_(number_of_trials),
			video_capture_(movie_filename.c_str()),
			tms_shock_absolute_time_(finger_start_moving_time+tms_shock_rational_time){
		init_serial(serial_com_id);
		if(!video_capture_.isOpened()){
			throw std::invalid_argument("movie file name is invalid");
		}
		double fps = video_capture_.get(CV_CAP_PROP_FPS);
		msec_per_frame_ = 1000.0/fps;
		cv::namedWindow(window_name_.c_str(), CV_WINDOW_NORMAL);
        //cv::setWindowProperty(window_name_.c_str(), CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		offset_wait_time_ = 
			5.0-1.0-video_capture_.get(CV_CAP_PROP_FRAME_COUNT)/fps;
		if(offset_wait_time_ < 0){
			throw std::invalid_argument("movie length is too long");
		}
	}

	auto run() -> void {
		for(std::size_t i = 0; i < number_of_trials_; ++i){
			std::cout << "trial: " << i << "-----------------" << std::endl;
            auto is_continue = trial();
            if(!is_continue){ break; }
		}
        //cv::destroyWindow(window_name_.c_str());
        cv::destroyAllWindows();
        //cv::waitKey(100);
        serial_port_.close();
        io_service_.stop();
	}

private:
	auto init_serial(const std::string& serial_com_id) -> void {
        serial_port_.open(serial_com_id.c_str());
	}

    auto trial() -> bool {
		video_capture_.set(CV_CAP_PROP_POS_FRAMES, 0);
		cv::Mat frame;
		video_capture_ >> frame;
		if(frame.empty()){
			throw std::invalid_argument("movie file is empty");
		}
		cv::imshow(window_name_.c_str(), frame);
		cv::waitKey(msec_per_frame_);
        io_service_.post(sound_player_);
		boost::asio::deadline_timer timer(io_service_, boost::posix_time::seconds(1));
		timer.wait();
		boost::asio::deadline_timer timer2(io_service_, 
				boost::posix_time::seconds(tms_shock_absolute_time_));
		timer2.async_wait([this](const boost::system::error_code&){
			std::cout << "send byte" << std::endl;
			serial_port_.write_some(boost::asio::buffer(&serial_byte_.front(), 1));	
		});
		while(true){
			video_capture_ >> frame;
			if(frame.empty()){
				break;
			}
			cv::imshow(window_name_.c_str(), frame);
			if(cv::waitKey(msec_per_frame_) == 27/*ESC*/){
                return false;
			}
		}	
		boost::asio::deadline_timer timer3(io_service_, 
				boost::posix_time::seconds(offset_wait_time_));
		timer3.wait();
        return true;
	}
	
	boost::asio::io_service& io_service_;
    std::function<void ()> sound_player_;
	boost::asio::serial_port serial_port_;
	std::size_t number_of_trials_;
	cv::VideoCapture video_capture_;
	double msec_per_frame_;
	double offset_wait_time_;
	double tms_shock_absolute_time_;
	static const std::array<char, 1> serial_byte_;
	static const std::string window_name_;

};
const std::array<char, 1> tms_shocker::serial_byte_ = {{2}};
const std::string tms_shocker::window_name_ = "shocker";
}

#endif //SHOCKER_SHOCKER_H
