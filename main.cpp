#include <iostream>
#include <memory>
#include <cmath>

// Класс трехмерного вектора
class Vector3D {
private:
    // Динамический массив из 3-х элементов под управлением уникального умного указателя
    std::unique_ptr<double[]> coords;

public:
    // 1. Конструктор по умолчанию и параметризованный конструктор
    Vector3D(double x = 0.0, double y = 0.0, double z = 0.0) {
        coords = std::make_unique<double[]>(3);
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
        std::cout << "[Конструктор] Создан вектор (" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")\n";
    }

    // Не нужно писать деструктор — std::unique_ptr автоматически очистит память

    // 2. Вручную реализуем Конструктор копирования (Deep Copy / Глубокое копирование)
    Vector3D(const Vector3D& other) {
        coords = std::make_unique<double[]>(3);
        for (int i = 0; i < 3; ++i) {
            coords[i] = other.coords[i];
        }
        std::cout << "[Копирование] Сработал конструктор копирования\n";
    }

    // 3. Вручную реализуем Оператор присваивания копированием
    Vector3D& operator=(const Vector3D& other) {
        std::cout << "[Копирование] Сработал оператор присваивания копированием\n";
        if (this != &other) {
            for (int i = 0; i < 3; ++i) {
                coords[i] = other.coords[i];
            }
        }
        return *this;
    }

    /// 4. Реализация семантики перемещения
// Используем '= default', так как std::unique_ptr автоматически корректно 
// перемещает ресурсы, обнуляя старый указатель и предотвращая утечки памяти.
    Vector3D(Vector3D&& other) noexcept = default;
    Vector3D& operator=(Vector3D&& other) noexcept = default;

    // Метод вывода координат вектора в консоль
    void print() const {
        if (coords) {
            std::cout << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")\n";
        } else {
            std::cout << "(пустой вектор, ресурсы перемещены)\n";
        }
    }

    // Метод нормализации вектора (изменение длины вектора до 1)
    void normalize() {
        if (!coords) return;
        
        // Вычисляем длину вектора по формуле: sqrt(x^2 + y^2 + z^2)
        double length = std::sqrt(coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2]);
        
        if (length > 0) {
            coords[0] /= length;
            coords[1] /= length;
            coords[2] /= length;
        }
    }
};

// Функция, принимающая std::shared_ptr по значению
void processVector(std::shared_ptr<Vector3D> vec) {
    std::cout << "\n[shared_ptr функция] Начало работы. Счетчик ссылок внутри функции = " << vec.use_count() << "\n";
    std::cout << "[shared_ptr функция] Исходный вектор: ";
    vec->print();
    
    std::cout << "[shared_ptr функция] Выполняем нормализацию вектора...\n";
    vec->normalize();
    
    std::cout << "[shared_ptr функция] Вектор после нормализации: ";
    vec->print();
    std::cout << "[shared_ptr функция] Завершение работы. Счетчик ссылок перед выходом = " << vec.use_count() << "\n";
}

// Демонстрация и тестирование
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cout << "=== ТЕСТ 1: СЕМАНТИКА ПЕРЕМЕЩЕНИЯ ===\n";
    Vector3D v1(3.0, 4.0, 0.0);
    std::cout << "v1 до перемещения: "; v1.print();

    // Тестируем конструктор перемещения при помощи std::move
    std::cout << "\n--> Выполняем: Vector3D v2 = std::move(v1);\n";
    Vector3D v2 = std::move(v1); 
    std::cout << "v1 (после перемещения): "; v1.print();
    std::cout << "v2 (новый владелец ресурсов): "; v2.print();
    
    Vector3D v3;
    // Тестируем оператор присваивания перемещением при помощи std::move
    std::cout << "\n--> Выполняем: v3 = std::move(v2);\n";
    v3 = std::move(v2);
    std::cout << "v2 (после перемещения): "; v2.print();
    std::cout << "v3 (новый владелец ресурсов): "; v3.print();


    std::cout << "\n=== ТЕСТ 2: РАБОТА С УМНЫМИ УКАЗАТЕЛЯМИ ===\n";
    
    // Демонстрация работы std::unique_ptr
    std::cout << "--> Создаем объект через std::unique_ptr:\n";
    std::unique_ptr<Vector3D> uPtr = std::make_unique<Vector3D>(1.0, 2.0, 3.0);
    std::cout << "Вектор под управлением unique_ptr: "; uPtr->print();
    // При выходе из области видимости main uPtr автоматически удалит Vector3D

    // Демонстрация работы std::shared_ptr и передачи в функцию
    std::cout << "\n--> Создаем объект через std::shared_ptr:\n";
    std::shared_ptr<Vector3D> sPtr = std::make_shared<Vector3D>(0.0, 5.0, 0.0);
    std::cout << "Счетчик ссылок в main до вызова функции = " << sPtr.use_count() << "\n";
    
    // Передаем shared_ptr в функцию processVector
    processVector(sPtr); 
    
    std::cout << "\nСчетчик ссылок в main после выхода из функции = " << sPtr.use_count() << "\n";
    std::cout << "Вектор sPtr в main после работы функции (изменения сохранились): ";
    sPtr->print();

    std::cout << "\n=== Завершение программы (сейчас сработают автоматические деструкторы умных указателей) ===\n";
    return 0;
}
