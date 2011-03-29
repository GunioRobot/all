std::ifstream stream;

void readbytes(void* ptr, unsigned int bytes);
void open(const char* file) {
	stream.open(file);
	if (stream) parse()
	else error();
}

void handle() {
	case MESH : enter()
}
void parse() {

}

void error() {

}
