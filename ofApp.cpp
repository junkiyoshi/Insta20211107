#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(0.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int z = -1500; z <= 0; z += 15) {

		ofColor face_color = ofColor(ofMap(z, -1500, 0, 255, 0));
		ofColor frame_color = ofColor(39, ofMap(z, -1500, 0, 0, 255));

		this->setRingToMesh(this->face, this->frame, glm::vec3(0, 0, z), 300, 75, 15, face_color, frame_color);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofRotateX(ofMap(sin(ofGetFrameNum() * 0.02), -1, 1, -12.5, 12.5));
	ofRotateY(ofMap(cos(ofGetFrameNum() * 0.02), -1, 1, -12.5, 12.5));

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float width, float height, ofColor face_color, ofColor frame_color) {

	int index = face_target.getNumVertices();

	for (int param = 0; param < 100; param ++) {

		float deg = param * 3.6 - 135;
		float deg_span = 3.6;

		auto noise_location = location + glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0);
		auto next_noise_location = location + glm::vec3(radius * cos((deg + deg_span) * DEG_TO_RAD), radius * sin((deg + deg_span) * DEG_TO_RAD), 0);

		int inner_radius = radius - width * ofMap(ofNoise(glm::vec4(noise_location.x * 0.002, noise_location.y * 0.002, noise_location.z * 0.005, ofGetFrameNum() * 0.015)), 0, 1, 0, 2.5);
		int next_inner_radius = radius - width * ofMap(ofNoise(glm::vec4(next_noise_location.x * 0.002, next_noise_location.y * 0.002, next_noise_location.z * 0.005, ofGetFrameNum() * 0.015)), 0, 1, 0, 2.5);

		auto face_index = face_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(inner_radius * cos(deg * DEG_TO_RAD), inner_radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(next_inner_radius * cos((deg + deg_span) * DEG_TO_RAD), next_inner_radius * sin((deg + deg_span) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(next_inner_radius * cos((deg + deg_span) * DEG_TO_RAD), next_inner_radius * sin((deg + deg_span) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(inner_radius * cos(deg * DEG_TO_RAD), inner_radius * sin(deg * DEG_TO_RAD), height * 0.5));

		vertices.push_back(glm::vec3(this->make_point(radius * 2, param), height * -0.5));
		vertices.push_back(glm::vec3(this->make_point(radius * 2, param + 1), height * -0.5));
		vertices.push_back(glm::vec3(this->make_point(radius * 2, param + 1), height * 0.5));
		vertices.push_back(glm::vec3(this->make_point(radius * 2, param), height * 0.5));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 6);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2);

		auto frame_index = frame_target.getNumVertices();

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);

		for (int i = 0; i < 8; i++) {

			this->face.addColor(face_color);
			this->frame.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}