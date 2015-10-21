//This file is part of Bertini 2.0.
//
//logging.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//logging.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with logging.hpp.  If not, see <http://www.gnu.org/licenses/>.
//

//  logging.hpp
//
//  copyright 2015
//  Daniel Brake
//  University of Notre Dame
//  ACMS
//  Fall 2015


#ifndef BERTINI_LOGGING_HPP
#define BERTINI_LOGGING_HPP 

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace bertini
{


	namespace logging = boost::log;
	namespace src = boost::log::sources;
	namespace sinks = boost::log::sinks;
	namespace keywords = boost::log::keywords;

	struct LoggingInit
	{
		LoggingInit()
		{
			logging::add_file_log
			(
			    keywords::file_name = "bertini_%N.log",
			    keywords::rotation_size = 10 * 1024 * 1024,
			    keywords::format = "[%TimeStamp%]: %Message%"
			);

			logging::core::get()->set_filter
			(
			    logging::trivial::severity >= logging::trivial::info
			);
		}

		~LoggingInit(){}
	    
	};


} // re: namespace bertini



#endif
