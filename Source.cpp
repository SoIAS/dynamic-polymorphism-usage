#include <iostream>
#include <stdexcept>
#include <random>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

class Figure
{
public:
	virtual ~Figure() = default;

	virtual double area() const noexcept = 0;
	virtual double circumference() const noexcept = 0;

	virtual void print() const = 0;
};

void Figure::print() const
{
	std::cout << "Area of " << typeid(*this).name() << " is " << area() << '\n';
	std::cout << "Circumference of " << typeid(*this).name() << " is " << circumference() << '\n';
}

class Square final : public Figure
{
public:
	Square(const double side_length) :
		side_length_{ 1 }
	{
		set_side_length(side_length);
	}

	Square(const Square&) = default;
	Square(Square&&) noexcept = default;

	Square& operator=(const Square&) = default;
	Square& operator=(Square&&) noexcept = default;

	double area() const noexcept override
	{
		return pow(side_length(), 2);
	}

	double circumference() const noexcept override
	{
		constexpr int amount_of_sides = 4;
		return side_length() * amount_of_sides;
	}

	void print() const override
	{
		std::cout << "Side length of the square is " << side_length() << '\n';
		Figure::print();
	}

	void set_side_length(const double side_length)
	{
		if(side_length <= 0)
		{
			throw std::range_error{ "Side length cannot be <= 0" };
		}

		side_length_ = side_length;
	}

	double side_length() const noexcept
	{
		return side_length_;
	}

private:
	double side_length_;
};

class Circle final : public Figure
{
public:
	Circle(const double radius) :
		radius_{ 1 }
	{
		set_radius(radius);
	}

	Circle(const Circle&) = default;
	Circle(Circle&&) noexcept = default;

	Circle& operator=(const Circle&) = default;
	Circle& operator=(Circle&&) noexcept = default;

	double area() const noexcept override
	{
		return M_PI * pow(radius(), 2);
	}

	double circumference() const noexcept override
	{
		return 2 * radius() * M_PI;
	}

	void print() const override
	{
		std::cout << "Radius of the circle is " << radius() << '\n';
		Figure::print();
	}

	void set_radius(const double radius)
	{
		if (radius <= 0)
		{
			throw std::range_error{ "Radius cannot be <= 0" };
		}

		radius_ = radius;
	}

	double radius() const noexcept
	{
		return radius_;
	}

private:
	double radius_;
};

int main()
{
	constexpr int amount_of_figures{ 10 };

	std::default_random_engine engine{};
	const std::bernoulli_distribution bool_dist{ 0.5 };
	const std::uniform_real_distribution<double> real_dist{1, 10};

	std::vector<std::unique_ptr<Figure>> figures;
	figures.reserve(amount_of_figures);
	for(int i = 0; i < amount_of_figures; ++i)
	{
		std::unique_ptr<Figure> figure;
		if(bool_dist(engine))
		{
			figure = std::make_unique<Circle>(real_dist(engine));
		}
		else
		{
			figure = std::make_unique<Square>(real_dist(engine));
		}


		figures.push_back(std::move(figure));
	}

	for(const auto& figure : figures)
	{
		if(auto square = dynamic_cast<Square*>(figure.get()))
		{
			square->set_side_length(square->side_length() + 5);
		}

		std::cout << figure->area() << " " << figure->circumference() << '\n';

		// another print
		figure->print();
		std::cout << '\n';
	}

	std::cin.get();
}


// composition example
class Engine
{
	//...
};

class Car
{
public:
	Car(std::shared_ptr<Engine> engine) :
		engine_{ std::move(engine) }
	{
		
	}
	virtual ~Car() = default;

	Car(const Car&) = default;
	Car(Car&&) = default;

	Car& operator=(const Car&) = default;
	Car& operator=(Car&&) = default;

private:
	std::shared_ptr<Engine> engine_;
};