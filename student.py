#!/bin/bash python3

class Student:
    ...


def main():
    student = get_student()
    #if student["name"] == "Padma":
    #    student["house"] == "Ravenclav":
    #print(f"{student['name']} from {student['house']} ")
    print(f"{student.name} from {student.house} ")


def get_student():
    student = Student()
    student.name = input("Name: ")
    student.house = input("House: ")
    return student



if __name__ == "__main__":
    main()