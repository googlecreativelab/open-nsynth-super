/*
Copyright 2017 Google Inc. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Patch.h"


ofJson Patch::toJson(){
	ofJson node;
	node["position"] = position;
	node["attack"] = attack;
	node["decay"] = decay;
	node["sustain"] = sustain;
	node["release"] = release;

	ofJson instsNode;
	for(auto& instrument : instruments){
		instsNode.push_back(instrument);
	}
	node["instruments"] = instsNode;

	node["grid"] = {grid[0], grid[1]};
	return node;
}


bool Patch::fromJson(const ofJson &node){
	bool result = true;
	auto readFloat = [&result](const ofJson &node, float &value){
		if(node.is_number()){
			value = std::min(std::max(node.get<float>(), 0.0f), 1.0f);
		}else{
			result = false;
		}
	};

	readFloat(node["position"], position);
	readFloat(node["attack"], attack);
	readFloat(node["decay"], decay);
	readFloat(node["sustain"], sustain);
	readFloat(node["release"], release);

	const ofJson &instsNode = node["instruments"];
	if(instsNode.is_array() && instsNode.size() == instruments.size()){
		for(size_t i=0; i<instruments.size(); ++i){
			instruments[i] = instsNode[i];
		}
	}else{
		result = false;
	}

	const ofJson &gridNode = node["grid"];
	if(gridNode.is_array() && gridNode.size() == grid.size()){
		for(size_t i=0; i<grid.size(); ++i){
			readFloat(gridNode[i], grid[i]);
		}
	}else{
		result = false;
	}

	return result;
}


bool Patches::load(){
	ofJson doc = ofLoadJson(filename);
	if(doc.is_null()){
		return false;
	}

	size_t idx = 0;
	bool result = true;
	for(auto& patchNode : doc["patches"]){
		if(idx>=size()){
			break;
		}
		result = (*this)[idx].fromJson(patchNode) && result;
		++idx;
	}

	return result;
}


bool Patches::save(){
	ofJson doc;
	ofJson patchesNode;
	for(auto& patch : *this){
		patchesNode.push_back(patch.toJson());
	}
	doc["patches"] = patchesNode;
	return ofSaveJson(filename, doc);
}
