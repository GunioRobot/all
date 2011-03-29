#ifndef _C_Mesh_H_
	#define _C_Mesh_H_

#include <string>
#include <vector>

namespace crucial {
	struct CMesh {
		std::string name;
		std::vector<unsigned short> indices;
		std::vector<float> texcoords;
		std::vector<float> vertices;
	};

	#ifdef BOOST_SHARED_PTR_HPP_INCLUDED
		typedef boost::shared_ptr<CMesh> CMesh_ptr;
	#endif
}

#endif
