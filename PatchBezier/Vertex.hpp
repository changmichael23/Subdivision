

#include "Edge.hpp"
#include "Face.hpp"
class Vertex {
	// each point connected to edges
private:
	Point point;
	
	std::vector<Face*> adjacent_faces;
	std::vector<Edge*> * edgesConnected;
	Colore color;

public:
	Vertex() {
		edgesConnected = new std::vector<Edge*>();
	}
	Vertex(Point p)
	{
		point = p;
		edgesConnected = new std::vector<Edge*>();
	}
	Vertex(Colore c, Point p)
	{
		color = c;
		point = p;
	}

	Vertex(Colore c, std::vector<Edge*> *e, Point p)
	{
		color = c;

		edgesConnected = e;
		point = p;
	}

	void setPoint(const Point & p)
	{
		point = p;
	}

	Point getPoint()
	{
		return point;
	}

	void setColor(const Colore & c)
	{
		color = c;
	}

	const Colore & getColor() const
	{
		return color;
	}

	void setEdgesConnected(std::vector<Edge*> * f)
	{
		edgesConnected = f;
	}

	std::vector<Edge*> * getEdgesConnected()
	{
		return edgesConnected;
	}

};