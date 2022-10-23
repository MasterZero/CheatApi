#pragma once
class cheat_api {
private:

	std::map <std::string, uintptr_t> modules{
		{"GameAssembly.dll",0},
		{"UnityPlayer.dll",0}
	};

public:

	static inline dll_api* driver = nullptr;
	
	auto get_module_base(std::string name)->uintptr_t {
		return modules[name];
	}

	auto test() {
		uintptr_t ret = 0;
		while (ret ==0)ret = driver->read<uintptr_t>(modules["GameAssembly.dll"]);
		return ret;
	}

	cheat_api() {
		driver = new dll_api("Dll1.dll");
		driver->setup_tprc(driver->get_md_pid(modules));
	}

	~cheat_api() {
		if(driver)
			delete driver;
	}
};