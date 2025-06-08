#pragma once
#include <string>

// Хранит информацию о категориях расходов
// Позволяет группировать траты и строить отчёты «по категориям»
// При желании можно расширить до иерархии (поле parentId) для вложенных категорий (например, Еда -> Ресторан)


class Category {
private:
	std::string name_;		// Name (for example, "products", "cafe", "transport")
	int id_;				// Unique Category ID
	int parent_id_;			// ID of the parental category (or -1, if not)

public:
	Category(int id, const std::string& name, int parent_id = -1) : name_(name), id_(id), parent_id_(parent_id) {}
	virtual ~Category() = default;

	const std::string& get_name() const{ return name_; }
	int get_id() const { return id_; }
	int get_parent_id() const { return parent_id_; }
};