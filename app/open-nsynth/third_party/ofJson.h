/*
	Copyright (c) 2004 - openFrameworks Community

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "json.hpp"
#include "ofFileUtils.h"
#include "ofParameter.h"

// for convenience
using ofJson = nlohmann::json;


/// \brief Load Json from the given path.
/// \param filename The file to load from.
/// \returns loaded json, or an empty json object on failure.
inline ofJson ofLoadJson(const std::filesystem::path& filename){
	ofJson json;
	ofFile jsonFile(filename);
	if(jsonFile.exists()){
		try{
			jsonFile >> json;
		}catch(std::exception & e){
			ofLogError("ofLoadJson") << "Error loading json from " << filename.string() << ": " << e.what();
		}catch(...){
			ofLogError("ofLoadJson") << "Error loading json from " << filename.string();
		}
	}else{
		ofLogError("ofLoadJson") << "Error loading json from " << filename.string() << ": file doesn't exist";
	}
	return json;
}

/// \brief Save minified Json to the given path.
/// \param filename The destination path.
/// \param json The Json to save.
/// \returns true if the json was saved successfully.
inline bool ofSaveJson(const std::filesystem::path& filename, const ofJson & json){
	ofFile jsonFile(filename, ofFile::WriteOnly);
	try{
		jsonFile << json;
	}catch(std::exception & e){
		ofLogError("ofLoadJson") << "Error saving json to " << filename.string() << ": " << e.what();
		return false;
	}catch(...){
		ofLogError("ofLoadJson") << "Error saving json to " << filename.string();
		return false;
	}
	return true;
}

/// \brief Save "pretty" indented Json to the given path.
/// \param filename The destination path.
/// \param json The Json to save.
/// \returns true if the json was saved successfully.
inline bool ofSavePrettyJson(const std::filesystem::path& filename, const ofJson & json){
    ofFile jsonFile(filename, ofFile::WriteOnly);
    try{
        jsonFile << json.dump(4);
    }catch(std::exception & e){
        ofLogError("ofLoadJson") << "Error saving json to " << filename.string() << ": " << e.what();
        return false;
    }catch(...){
        ofLogError("ofLoadJson") << "Error saving json to " << filename.string();
        return false;
    }
    return true;
}

inline void ofSerialize(ofJson & js, ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	if(name == ""){
		name = "UnknownName";
	}
	if(parameter.type() == typeid(ofParameterGroup).name()){
		const ofParameterGroup & group = static_cast <const ofParameterGroup &>(parameter);
		auto jsonGroup = js[name];
		for(auto & p: group){
			ofSerialize(jsonGroup, *p);
		}
		js[name] = jsonGroup;
	}else{
		string value = parameter.toString();
		js[name] = value;
	}
}

inline void ofDeserialize(const ofJson & json, ofAbstractParameter & parameter){
	if(!parameter.isSerializable()){
		return;
	}
	string name = parameter.getEscapedName();
	if(json.find(name) != json.end()){
		if(parameter.type() == typeid(ofParameterGroup).name()){
			ofParameterGroup & group = static_cast <ofParameterGroup &>(parameter);
			for(auto & p: group){
				ofDeserialize(json[name], *p);
			}
		}else{
			if(parameter.type() == typeid(ofParameter <int> ).name() && json[name].is_number_integer()){
				parameter.cast <int>() = json[name].get<int>();
			}else if(parameter.type() == typeid(ofParameter <float> ).name() && json[name].is_number_float()){
				parameter.cast <float>() = json[name].get<float>();
			}else if(parameter.type() == typeid(ofParameter <bool> ).name() && json[name].is_boolean()){
				parameter.cast <bool>() = json[name].get<bool>();
			}else if(parameter.type() == typeid(ofParameter <int64_t> ).name() && json[name].is_number_integer()){
				parameter.cast <int64_t>() = json[name].get<int64_t>();
			}else if(parameter.type() == typeid(ofParameter <string> ).name()){
				parameter.cast <std::string>() = json[name].get<std::string>();
			}else{
				parameter.fromString(json[name]);
			}
		}
	}
}
