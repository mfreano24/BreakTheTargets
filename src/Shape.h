#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <vector>
#include <memory>

class Program;

/**
 * A shape defined by a list of triangles
 * - posBuf should be of length 3*ntris
 * - norBuf should be of length 3*ntris (if normals are available)
 * - texBuf should be of length 2*ntris (if texture coords are available)
 * posBufID, norBufID, and texBufID are OpenGL buffer identifiers.
 */
class Shape
{
public:
	Shape();
	virtual ~Shape();
	void loadMesh(const std::string &meshName);
	void fitToUnitBox();
	void init();
	void draw(const std::shared_ptr<Program> prog) const;

	void CopyData(std::vector<float>& _posBuf, std::vector<float>& _norBuf){
		posBuf = _posBuf;
		norBuf = _norBuf;
	}
	
private:
	std::vector<float> posBuf;
	std::vector<float> norBuf;
	std::vector<int> indBuf;
	unsigned posBufID;
	unsigned norBufID;
	unsigned texBufID;
};

#endif
