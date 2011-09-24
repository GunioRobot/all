////////////////////////////
// usage
UnitConfig US_Soldier_B = {"shared/models/us_soldier_b.mf"};

save_unit_config("us_soldier_b.ucf", US_Soldier_B);

UnitConfig US_Test_B;
load_unit_config("us_soldier_b.ucf", US_Test_B);

assert(US_Test_B.model_name.compare(US_Soldier_B.model_name) == 0);

////////////////////////////
//.h
#include "std/containers.h"

struct UnitConfig {
	string model_name;
};

void load_unit_config(const char* file_name, UnitConfig& config);
void save_unit_config(const char* file_name, const UnitConfig& config);



////////////////////////////
// .cpp
#include "ucf.h"

#include "std/assert.h"
#include "std/io.h"
#include "std/types.h"

void load_unit_config(const char* file_name, UnitConfig& config) {
	File file(file_name, "rb");
	assert(file.isopen());

	ubyte strlen;
	file.read(&strlen, sizeof(strlen));

	// pre-allocate
	config.model_name.resize(strlen);

	// read data
	file.read(&config.model_name[0], sizeof(config.model_name[0]), strlen);
}

void save_unit_config(const char* file_name, const UnitConfig& config) {
	File file(file_name, "wb");
	assert(file.isopen());

	ubyte strlen = (ubyte) config.model_name.size();

	// write data
	file.write(&strlen, sizeof(strlen));
	file.write(&config.model_name[0], sizeof(config.model_name[0]), strlen);
}
