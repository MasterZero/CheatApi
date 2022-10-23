#pragma once

class dll_api {
private:

	HMODULE DLL_BASE = 0;
	uint32_t target = 0;

	std::map<char, std::pair<std::string, FARPROC>> dll_exp{
		{'r', {"usermode_read", 0}},
		{'w', {"usermode_write", 0}},
		{'m', {"usermode_enum_modules", 0}},
		{'g', {"run_get_modules_ret_pid", 0}}
	};

public:

	auto& get_tprc() { return this->target; }
	auto setup_tprc(const uint32_t target) -> void { this->target = target; }

	template<typename Type>
	auto read(const uintptr_t address) -> Type {

		Type buffer{  };

		reinterpret_cast<bool(*) (
			uint32_t, // process id
			uintptr_t,// target va
			void*,	  // buffer va
			size_t	  // copy size
			)>(this->dll_exp['r'].second)(this->target, address, &buffer, sizeof(Type));

		return buffer;
	}

	template<typename Type>
	auto write(const uintptr_t address, Type buffer) -> void {

		reinterpret_cast<bool(*) (
			uint32_t, // process id
			uintptr_t,// target va
			void*,	  // buffer va
			size_t	  // copy size
			)>(this->dll_exp['w'].second)(this->target, address, &buffer, sizeof(Type));

		//return;
	}

	auto enum_modeles(const uint32_t max_count, std::vector<std::pair<std::wstring, std::wstring>>& result) -> void {

		reinterpret_cast<bool(*) (
			uint32_t, // process id
			uint32_t, // max_count
			std::vector<std::pair<std::wstring, std::wstring>>&
			)>(this->dll_exp['m'].second)(this->target, max_count, result);

		//return
	}

	auto get_md_pid(std::map <std::string, uintptr_t>& nmap) ->uint32_t {

		return 
			reinterpret_cast<uint32_t(*) (
			std::map <std::string, uintptr_t>&
			)>(this->dll_exp['g'].second)(nmap);

	}

	template<typename Type>
	auto read_chain(uintptr_t base, std::vector<uintptr_t> offsets)->Type {

		uintptr_t last_ptr = base;

		for (auto i = 0; i < offsets.size(); i++) 
			if (i == (offsets.size() - 1))
				return read<Type>(last_ptr + offsets.at(i));
			else
				last_ptr = read<uintptr_t>(last_ptr + offsets.at(i));
		
		return Type();
	}

	dll_api(const char* dll_name) {

		this->DLL_BASE = LoadLibrary(dll_name);

		if (!this->DLL_BASE)
			return;

		for (auto& d_exp : this->dll_exp)
			d_exp.second.second = GetProcAddress(this->DLL_BASE, d_exp.second.first.c_str());

	}
};

