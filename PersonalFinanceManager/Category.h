#pragma once
#include <string>
#include <chrono>

// Stores information about cost categories
// Allows you to group expenses and build reports "by categories"
// If desired, you can expand to the hierarchy (field ID) for invested categories (for example, food -> restaurant)


class Category {
private:
	std::string name_;	// Name (for example, "products", "cafe", "transport")
	int id_;			// Unique Category ID
	int parent_id_;		// ID of the parental category (or -1, if not)

public:
	// Constructor and Destructor
	Category(const std::string& name, int id, int parent_id = -1) : name_(name), id_(id), parent_id_(parent_id) {}
	virtual ~Category() = default;

	// Getters
	const std::string& get_name() const { return name_; }
	int get_id() const { return id_; }
	int get_parent_id() const { return parent_id_; }

	// Setters
	void set_name(std::string new_name) { name_ = new_name; }
	void set_parent_id(int new_parent_id) { parent_id_ = new_parent_id; }
};