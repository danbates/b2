//This file is part of Bertini 2.
//
//amp_tracker.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//amp_tracker.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with amp_tracker.hpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015, 2016 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license,
// as well as COPYING.  Bertini2 is provided with permitted
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// James Collins, West Texas A&M University


/**
 \file config_parsing.hpp
 
 \brief Contains qi parsers and functions needed to parse the config settings from a classic Bertini input file
 */

#ifndef config_parsing_hpp
#define config_parsing_hpp

#include <boost/fusion/adapted.hpp>
#include <boost/fusion/include/adapted.hpp>


#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_DEBUG_PRINT_SOME 80
#define BOOST_SPIRIT_USE_PHOENIX_V3 1


#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_no_case.hpp>

#include <boost/spirit/include/support_istream_iterator.hpp>

#include <bertini2/Tracking/tracking_config.hpp>


#include <iostream>
#include <string>


namespace bertini
{
	namespace classic
	{
		namespace parsing
		{
			using namespace bertini::tracking;
			
			
			
			// Struct that holds the rules for parsing mpfr numbers
			template<typename Iterator>
			struct MPParserRules
			{
				MPParserRules()
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;

					integer_string_.name("integer_string_");
					integer_string_ = eps[_val = std::string()] >>
					number_with_no_point_ [_val += _1];
					
					
					long_number_string_.name("long_number_string_");
					long_number_string_ = eps[_val = std::string()] >>
					(
					 // 1. Read possible numbers before decimal, with possible negative
					 (number_with_digits_after_point_ [_val += _1]
					 |
					 number_with_digits_before_point_ [_val += _1] )
					 >>   // reminder -- the - before the exponent_notation here means optional
					 -exponent_notation_ [_val+=_1]// Possible scientific notation, with possible negative in exponent.
					 );
					
					
					
					number_with_digits_after_point_.name("number_with_digits_after_point_");
					number_with_digits_after_point_ = eps[_val = std::string()]
					>>
					*(qi::char_(L'0',L'9')[_val += _1])
					>>
					qi::lit('.')[_val += "."] // find a decimal point
					>>
					+(qi::char_(L'0',L'9')[_val += _1]) // find at least one digit after the point
					;
					
					
					
					
					
					number_with_digits_before_point_.name("number_with_digits_before_point_");
					number_with_digits_before_point_ = eps[_val = std::string()]
					>>
					+(qi::char_(L'0',L'9')[_val += _1])
					>>
					qi::lit('.')[_val += "."] // find a decimal point
					>>
					*(qi::char_(L'0',L'9')[_val += _1]) // find any number of digits after the point
					;
					
					
					number_with_no_point_.name("number_with_no_point_");
					number_with_no_point_ = eps[_val = std::string()]
					>>
					+(qi::char_(L'0',L'9')[_val += _1])
					;
					
					
					
					
					exponent_notation_.name("exponent_notation_");
					exponent_notation_ = eps[_val = std::string()]
					>> ( // start what the rule actually does
						(
						 qi::lit('e')[_val += "e"] // get an opening 'e'
						 |
						 qi::lit('E')[_val += "e"] // get an opening 'e'
						 )
						>>
						-(qi::lit('-')[_val += "-"]) // then an optional minus sign
						>>
						+(qi::char_(L'0',L'9')[_val += _1]) // then at least one number
						); // finish the rule off

				}
				
				
				
				// these rules all produce strings which are fed into numbers.
				qi::rule<Iterator, std::string()> integer_string_, long_number_string_, number_with_digits_before_point_, number_with_digits_after_point_, number_with_no_point_, exponent_notation_;

			}; //re: MPParserRules
			
			
			
			/**
			 Qi Parser object for parsing config settings  This ensures we can provide backwards compatibility with Bertini Classic input files.
			 
			 To use this parser, construct an object of its type, then use it to parse.
			 
			 \code
			 System sys;
			 std::string str = "variable_group x, y, z; \nfunction f1, f2;\n  f1 = x*y*z;\n f2 = x+y+z;\n";
			 
			 std::string::const_iterator iter = str.begin();
			 std::string::const_iterator end = str.end();
			 
			 
			 bertini::SystemParser<std::string::const_iterator> S;
			 
			 
			 bool s = phrase_parse(iter, end, S,boost::spirit::ascii::space, sys);
			 
			 \endcode
			 
			 \brief Qi Parser object for parsing config file to determine MPType setting.
			 
			 */
			template<typename Iterator, typename Skipper = ascii::space_type> //boost::spirit::unused_type
			struct ConfigPrecisionTypeParser : qi::grammar<Iterator, PrecisionType(), Skipper>
			{
				
				ConfigPrecisionTypeParser() : ConfigPrecisionTypeParser::base_type(root_rule_, "ConfigPrecisionType")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					precisiontype_.add("0", PrecisionType::Fixed);
					precisiontype_.add("1", PrecisionType::Adaptive);
					precisiontype_.add("2", PrecisionType::Adaptive);
					

	
					
					root_rule_.name("ConfigPrecisionType_root_rule");
					
					root_rule_ = (config_name_[_val = _1] >> -no_setting_) | no_setting_[_val = PrecisionType::Adaptive];

					config_name_.name("precisiontype_");
					config_name_ = *(char_ - no_case["mptype:"]) >> no_case["mptype:"] >> precisiontype_[_val = _1] >> ';';
					
					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - no_case["mptype:"]);

					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
//					debug(root_rule_);
//					debug(config_name_);
//					debug(no_setting_);
//					debug(no_decl_);
//					BOOST_SPIRIT_DEBUG_NODES((root_rule_) (config_name_) )

					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config/input split parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					

	
				}
				
				
			private:
				qi::rule<Iterator, PrecisionType(), ascii::space_type > root_rule_, config_name_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_;
				
				qi::symbols<char,PrecisionType> precisiontype_;


			}; //re: PrecisionTypeParser
			
	
			
			
			
			
			
			
			/**
			 Qi Parser object for parsing config settings  This ensures we can provide backwards compatibility with Bertini Classic input files.
			 
			 To use this parser, construct an object of its type, then use it to parse.
			 
			 \code
			 System sys;
			 std::string str = "variable_group x, y, z; \nfunction f1, f2;\n  f1 = x*y*z;\n f2 = x+y+z;\n";
			 
			 std::string::const_iterator iter = str.begin();
			 std::string::const_iterator end = str.end();
			 
			 
			 bertini::SystemParser<std::string::const_iterator> S;
			 
			 
			 bool s = phrase_parse(iter, end, S,boost::spirit::ascii::space, sys);
			 
			 \endcode
			 
			 \brief Qi Parser object for parsing config file to determine the tracking predictor from ODEPredictor.
			 
			 */
			template<typename Iterator, typename Skipper = ascii::space_type> //boost::spirit::unused_type
			struct ConfigPredictorParser : qi::grammar<Iterator, config::Predictor(), Skipper>
			{
				
				ConfigPredictorParser() : ConfigPredictorParser::base_type(root_rule_, "ConfigPredictor")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					predictor_.add("-1", config::Predictor::Constant);
					predictor_.add("0", config::Predictor::Euler);
					predictor_.add("1", config::Predictor::Heun);
					predictor_.add("2", config::Predictor::RK4);
					predictor_.add("3", config::Predictor::Heun);
					predictor_.add("4", config::Predictor::RKNorsett34);
					predictor_.add("5", config::Predictor::RKF45);
					predictor_.add("6", config::Predictor::RKCashKarp45);
					predictor_.add("7", config::Predictor::RKDormandPrince56);
					predictor_.add("8", config::Predictor::RKVerner67);

					
					std::string setting_name = "odepredictor:";
					
					
					root_rule_.name("ConfigPredictor_root_rule");
					
					root_rule_ = (config_name_[_val = _1] >> -no_setting_) | no_setting_[_val = config::Predictor::RKF45];
					
					config_name_.name("predictor_");
					config_name_ = *(char_ - no_case[setting_name]) >> no_case[setting_name] >> predictor_[_val = _1] >> ';';
					
					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - no_case[setting_name]);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
					//					debug(root_rule_);
					//					debug(config_name_);
					//					debug(no_setting_);
					//					debug(no_decl_);
					//					BOOST_SPIRIT_DEBUG_NODES((root_rule_) (config_name_) )
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config/input split parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::Predictor(), ascii::space_type > root_rule_, config_name_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_;
				
				qi::symbols<char,config::Predictor> predictor_;
				
				
			}; //re: PredictorTypeParser

			
			
			
			
			
			
			
			/**
			 Qi Parser object for parsing config settings  This ensures we can provide backwards compatibility with Bertini Classic input files.
			 
			 To use this parser, construct an object of its type, then use it to parse.
			 
			 \code
			 System sys;
			 std::string str = "variable_group x, y, z; \nfunction f1, f2;\n  f1 = x*y*z;\n f2 = x+y+z;\n";
			 
			 std::string::const_iterator iter = str.begin();
			 std::string::const_iterator end = str.end();
			 
			 
			 bertini::SystemParser<std::string::const_iterator> S;
			 
			 
			 bool s = phrase_parse(iter, end, S,boost::spirit::ascii::space, sys);
			 
			 \endcode
			 
			 \brief Qi Parser object for parsing config file to determine the security settings.
			 
			 */
			template<typename Iterator, typename T, typename Skipper = ascii::space_type> //boost::spirit::unused_type
			struct ConfigSecurityParser : qi::grammar<Iterator, config::Security<T>(), Skipper>
			{
				
				
				
				
				ConfigSecurityParser() : ConfigSecurityParser::base_type(root_rule_, "ConfigSecurity")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					
					
					std::string level_name = "securitylevel:";
					std::string maxnorm_name = "securitymaxnorm:";
					
					
					root_rule_.name("ConfigSecurity_root_rule");
					
					root_rule_ = ((security_level_[phx::bind( [this](config::Security<T> & S, int l)
																	  {
																		  S.level = l;
																	  }, _val, _1 )]
					^ security_max_norm_[phx::bind( [this](config::Security<T> & S, T norm)
												   {
													   S.max_norm = norm;
												   }, _val, _1 )])
					>> -no_setting_) | no_setting_;
					
					all_names_ = no_case[level_name] | no_case[maxnorm_name];
					
					security_level_.name("security_level_");
					security_level_ = *(char_ - all_names_) >> no_case[level_name] >> qi::int_[_val = _1] >> ';';
					
					security_max_norm_.name("security_max_norm_");
					security_max_norm_ = *(char_ - all_names_) >> no_case[maxnorm_name]
					>> mpfr_rules.long_number_string_[phx::bind( [this](T & num, std::string str)
																{
																	num = bertini::NumTraits<T>::FromString(str);
																}, _val, _1 )] >> ';';

					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - all_names_);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
//					debug(security_level_);
//										debug(security_max_norm_);
					//					debug(config_name_);
					//					debug(no_setting_);
					//					debug(no_decl_);
					//					BOOST_SPIRIT_DEBUG_NODES((root_rule_) (config_name_) )
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config/input split parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::Security<T>(), ascii::space_type > root_rule_;
				qi::rule<Iterator, int(), ascii::space_type > security_level_;
				qi::rule<Iterator, T(), ascii::space_type > security_max_norm_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_, all_names_;
				MPParserRules<Iterator> mpfr_rules;
				
				
				
			}; //re: SecurityParser

			
			
			
			/**
			 Qi Parser object for parsing config settings  This ensures we can provide backwards compatibility with Bertini Classic input files.
			 
			 To use this parser, construct an object of its type, then use it to parse.
			 
			 \code
			 System sys;
			 std::string str = "variable_group x, y, z; \nfunction f1, f2;\n  f1 = x*y*z;\n f2 = x+y+z;\n";
			 
			 std::string::const_iterator iter = str.begin();
			 std::string::const_iterator end = str.end();
			 
			 
			 bertini::SystemParser<std::string::const_iterator> S;
			 
			 
			 bool s = phrase_parse(iter, end, S,boost::spirit::ascii::space, sys);
			 
			 \endcode
			 
			 \brief Qi Parser object for parsing config file to determine the tolerance settings.
			 
			 */
			template<typename Iterator, typename T, typename Skipper = ascii::space_type> //boost::spirit::unused_type
			struct ConfigToleranceParser : qi::grammar<Iterator, config::Tolerances<T>(), Skipper>
			{
				
				
				
				
				ConfigToleranceParser() : ConfigToleranceParser::base_type(root_rule_, "ConfigTolerance")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					
					
					std::string newton_before_name = "tracktolbeforeeg:";
					std::string newton_during_name = "tracktolduringeg:";
					std::string final_tol_name = "finaltol:";
					std::string path_trunc_name = "pathtruncationthreshold:";
					
					
					root_rule_.name("ConfigTolerances_root_rule");
					
					root_rule_ = ((newton_before_endgame_[phx::bind( [this](config::Tolerances<T> & S, T l)
															 {
																 S.newton_before_endgame = l;
															 }, _val, _1 )]
								   ^ newton_during_endgame_[phx::bind( [this](config::Tolerances<T> & S, T num)
																  {
																	  S.newton_during_endgame = num;
																  }, _val, _1 )]
								  ^ final_tol_[phx::bind( [this](config::Tolerances<T> & S, T num)
																  {
																	  S.final_tolerance = num;
																  }, _val, _1 )]
									^ path_trunc_threshold_[phx::bind( [this](config::Tolerances<T> & S, T num)
																	   {
																		   S.path_truncation_threshold = num;
																	   }, _val, _1 )])

								  >> -no_setting_)
								| no_setting_;
					
//					root_rule_ = eps >> omit[final_tol_];
					
					all_names_ = no_case[newton_before_name] | no_case[newton_during_name]| no_case[final_tol_name]| no_case[path_trunc_name];
					
					newton_before_endgame_.name("newton_before_endgame_");
					newton_before_endgame_ = *(char_ - all_names_) >> no_case[newton_before_name]
						>> mpfr_rules.long_number_string_[phx::bind( [this](T & num, std::string str)
																{
																	num = bertini::NumTraits<T>::FromString(str);
																}, _val, _1 )] >> ';';

					newton_during_endgame_.name("newton_during_endgame_");
					newton_during_endgame_ = *(char_ - all_names_) >> no_case[newton_during_name]
					>> mpfr_rules.long_number_string_[phx::bind( [this](T & num, std::string str)
																{
																	num = bertini::NumTraits<T>::FromString(str);
																}, _val, _1 )] >> ';';

					final_tol_.name("final_tol_");
					final_tol_ = *(char_ - all_names_) >> no_case[final_tol_name]
					>> mpfr_rules.long_number_string_[phx::bind( [this](T & num, std::string str)
																{
																	num = bertini::NumTraits<T>::FromString(str);
																}, _val, _1 )] >> ';';

					path_trunc_threshold_.name("path_trunc_threshold_");
					path_trunc_threshold_ = *(char_ - all_names_) >> no_case[path_trunc_name]
					>> mpfr_rules.long_number_string_[phx::bind( [this](T & num, std::string str)
																{
																	num = bertini::NumTraits<T>::FromString(str);
																}, _val, _1 )] >> ';';

					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - all_names_);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
					
//					debug(no_setting_);
//					debug(newton_before_endgame_);
//					debug(newton_during_endgame_);
//					debug(final_tol_);
//					debug(path_trunc_threshold_);
//					debug(mpfr_rules.long_number_string_);
//					debug(mpfr_rules.number_with_digits_after_point_);
//					debug(mpfr_rules.number_with_digits_before_point_);
//					debug(mpfr_rules.exponent_notation_);
//					BOOST_SPIRIT_DEBUG_NODES((root_rule_) (final_tol_) )
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config/input split parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::Tolerances<T>(), ascii::space_type > root_rule_;
				qi::rule<Iterator, T(), ascii::space_type > newton_before_endgame_, newton_during_endgame_,
					final_tol_, path_trunc_threshold_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_, all_names_;
				MPParserRules<Iterator> mpfr_rules;
				
				
				
			}; //re: SecurityParser

		} // re: namespace parsing
		
	} // re: namespace classic
	
} // re: namespace bertini


#endif
