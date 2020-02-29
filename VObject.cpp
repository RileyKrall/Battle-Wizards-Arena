// Fill out your copyright notice in the Description page of Project Settings.


#include "VObject.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVObject::AVObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EdgeVertexPoint = { FVector(.5, 0, 0),
						FVector(1, .5, 0),
						FVector(.5, 1, 0),
						FVector(0, .5, 0),
						FVector(.5, 0, 1),
						FVector(1, .5, 1),
						FVector(.5, 1, 1),
						FVector(0, .5, 1),
						FVector(0, 0, .5),
						FVector(1, 0, .5),
						FVector(1, 1, .5),
						FVector(0, 1, .5) };

	edgeMap = {	FNestedArray({-1, -1, -1}),
				FNestedArray({0, 8, 3}),
				FNestedArray({0, 1, 9}),
				FNestedArray({1, 8, 3, 9, 8, 1}),
				FNestedArray({1, 2, 10}),
				FNestedArray({0, 8, 3, 1, 2, 10}),
				FNestedArray({9, 2, 10, 0, 2, 9}),
				FNestedArray({2, 8, 3, 2, 10, 8, 10, 9, 8}),
				FNestedArray({3, 11, 2}),
				FNestedArray({0, 11, 2, 8, 11, 0}),
				FNestedArray({1, 9, 0, 2, 3, 11}),
				FNestedArray({1, 11, 2, 1, 9, 11, 9, 8, 11}),
				FNestedArray({3, 10, 1, 11, 10, 3}),
				FNestedArray({0, 10, 1, 0, 8, 10, 8, 11, 10}),
				FNestedArray({3, 9, 0, 3, 11, 9, 11, 10, 9}),
				FNestedArray({9, 8, 10, 10, 8, 11}),
				FNestedArray({4, 7, 8}),
				FNestedArray({4, 3, 0, 7, 3, 4}),
				FNestedArray({0, 1, 9, 8, 4, 7}),
				FNestedArray({4, 1, 9, 4, 7, 1, 7, 3, 1}),
				FNestedArray({1, 2, 10, 8, 4, 7}),
				FNestedArray({3, 4, 7, 3, 0, 4, 1, 2, 10}),
				FNestedArray({9, 2, 10, 9, 0, 2, 8, 4, 7}),
				FNestedArray({2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4}),
				FNestedArray({8, 4, 7, 3, 11, 2}),
				FNestedArray({11, 4, 7, 11, 2, 4, 2, 0, 4}),
				FNestedArray({9, 0, 1, 8, 4, 7, 2, 3, 11}),
				FNestedArray({4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1}),
				FNestedArray({3, 10, 1, 3, 11, 10, 7, 8, 4}),
				FNestedArray({1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4}),
				FNestedArray({4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3}),
				FNestedArray({4, 7, 11, 4, 11, 9, 9, 11, 10}),
				FNestedArray({9, 5, 4}),
				FNestedArray({9, 5, 4, 0, 8, 3}),
				FNestedArray({0, 5, 4, 1, 5, 0}),
				FNestedArray({8, 5, 4, 8, 3, 5, 3, 1, 5}),
				FNestedArray({1, 2, 10, 9, 5, 4}),
				FNestedArray({3, 0, 8, 1, 2, 10, 4, 9, 5}),
				FNestedArray({5, 2, 10, 5, 4, 2, 4, 0, 2}),
				FNestedArray({2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8}),
				FNestedArray({9, 5, 4, 2, 3, 11}),
				FNestedArray({0, 11, 2, 0, 8, 11, 4, 9, 5}),
				FNestedArray({0, 5, 4, 0, 1, 5, 2, 3, 11}),
				FNestedArray({2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5}),
				FNestedArray({10, 3, 11, 10, 1, 3, 9, 5, 4}),
				FNestedArray({4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10}),
				FNestedArray({5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3}),
				FNestedArray({5, 4, 8, 5, 8, 10, 10, 8, 11}),
				FNestedArray({9, 7, 8, 5, 7, 9}),
				FNestedArray({9, 3, 0, 9, 5, 3, 5, 7, 3}),
				FNestedArray({0, 7, 8, 0, 1, 7, 1, 5, 7}),
				FNestedArray({1, 5, 3, 3, 5, 7}),
				FNestedArray({9, 7, 8, 9, 5, 7, 10, 1, 2}),
				FNestedArray({10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3}),
				FNestedArray({8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2}),
				FNestedArray({2, 10, 5, 2, 5, 3, 3, 5, 7}),
				FNestedArray({7, 9, 5, 7, 8, 9, 3, 11, 2}),
				FNestedArray({9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11}),
				FNestedArray({2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7}),
				FNestedArray({11, 2, 1, 11, 1, 7, 7, 1, 5}),
				FNestedArray({9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11}),
				FNestedArray({5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0}),
				FNestedArray({11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0}),
				FNestedArray({11, 10, 5, 7, 11, 5}),
				FNestedArray({10, 6, 5}),
				FNestedArray({0, 8, 3, 5, 10, 6}),
				FNestedArray({9, 0, 1, 5, 10, 6}),
				FNestedArray({1, 8, 3, 1, 9, 8, 5, 10, 6}),
				FNestedArray({1, 6, 5, 2, 6, 1}),
				FNestedArray({1, 6, 5, 1, 2, 6, 3, 0, 8}),
				FNestedArray({9, 6, 5, 9, 0, 6, 0, 2, 6}),
				FNestedArray({5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8}),
				FNestedArray({2, 3, 11, 10, 6, 5}),
				FNestedArray({11, 0, 8, 11, 2, 0, 10, 6, 5}),
				FNestedArray({0, 1, 9, 2, 3, 11, 5, 10, 6}),
				FNestedArray({5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11}),
				FNestedArray({6, 3, 11, 6, 5, 3, 5, 1, 3}),
				FNestedArray({0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6}),
				FNestedArray({3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9}),
				FNestedArray({6, 5, 9, 6, 9, 11, 11, 9, 8}),
				FNestedArray({5, 10, 6, 4, 7, 8}),
				FNestedArray({4, 3, 0, 4, 7, 3, 6, 5, 10}),
				FNestedArray({1, 9, 0, 5, 10, 6, 8, 4, 7}),
				FNestedArray({10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4}),
				FNestedArray({6, 1, 2, 6, 5, 1, 4, 7, 8}),
				FNestedArray({1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7}),
				FNestedArray({8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6}),
				FNestedArray({7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9}),
				FNestedArray({3, 11, 2, 7, 8, 4, 10, 6, 5}),
				FNestedArray({5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11}),
				FNestedArray({0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6}),
				FNestedArray({9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6}),
				FNestedArray({8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6}),
				FNestedArray({5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11}),
				FNestedArray({0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7}),
				FNestedArray({6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9}),
				FNestedArray({10, 4, 9, 6, 4, 10}),
				FNestedArray({4, 10, 6, 4, 9, 10, 0, 8, 3}),
				FNestedArray({10, 0, 1, 10, 6, 0, 6, 4, 0}),
				FNestedArray({8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10}),
				FNestedArray({1, 4, 9, 1, 2, 4, 2, 6, 4}),
				FNestedArray({3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4}),
				FNestedArray({0, 2, 4, 4, 2, 6}),
				FNestedArray({8, 3, 2, 8, 2, 4, 4, 2, 6}),
				FNestedArray({10, 4, 9, 10, 6, 4, 11, 2, 3}),
				FNestedArray({0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6}),
				FNestedArray({3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10}),
				FNestedArray({6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1}),
				FNestedArray({9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3}),
				FNestedArray({8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1}),
				FNestedArray({3, 11, 6, 3, 6, 0, 0, 6, 4}),
				FNestedArray({6, 4, 8, 11, 6, 8}),
				FNestedArray({7, 10, 6, 7, 8, 10, 8, 9, 10}),
				FNestedArray({0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10}),
				FNestedArray({10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0}),
				FNestedArray({10, 6, 7, 10, 7, 1, 1, 7, 3}),
				FNestedArray({1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7}),
				FNestedArray({2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9}),
				FNestedArray({7, 8, 0, 7, 0, 6, 6, 0, 2}),
				FNestedArray({7, 3, 2, 6, 7, 2}),
				FNestedArray({2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7}),
				FNestedArray({2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7}),
				FNestedArray({1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11}),
				FNestedArray({11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1}),
				FNestedArray({8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6}),
				FNestedArray({0, 9, 1, 11, 6, 7}),
				FNestedArray({7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0}),
				FNestedArray({7, 11, 6}),
				FNestedArray({7, 6, 11}),
				FNestedArray({3, 0, 8, 11, 7, 6}),
				FNestedArray({0, 1, 9, 11, 7, 6}),
				FNestedArray({8, 1, 9, 8, 3, 1, 11, 7, 6}),
				FNestedArray({10, 1, 2, 6, 11, 7}),
				FNestedArray({1, 2, 10, 3, 0, 8, 6, 11, 7}),
				FNestedArray({2, 9, 0, 2, 10, 9, 6, 11, 7}),
				FNestedArray({6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8}),
				FNestedArray({7, 2, 3, 6, 2, 7}),
				FNestedArray({7, 0, 8, 7, 6, 0, 6, 2, 0}),
				FNestedArray({2, 7, 6, 2, 3, 7, 0, 1, 9}),
				FNestedArray({1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6}),
				FNestedArray({10, 7, 6, 10, 1, 7, 1, 3, 7}),
				FNestedArray({10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8}),
				FNestedArray({0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7}),
				FNestedArray({7, 6, 10, 7, 10, 8, 8, 10, 9}),
				FNestedArray({6, 8, 4, 11, 8, 6}),
				FNestedArray({3, 6, 11, 3, 0, 6, 0, 4, 6}),
				FNestedArray({8, 6, 11, 8, 4, 6, 9, 0, 1}),
				FNestedArray({9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6}),
				FNestedArray({6, 8, 4, 6, 11, 8, 2, 10, 1}),
				FNestedArray({1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6}),
				FNestedArray({4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9}),
				FNestedArray({10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3}),
				FNestedArray({8, 2, 3, 8, 4, 2, 4, 6, 2}),
				FNestedArray({0, 4, 2, 4, 6, 2}),
				FNestedArray({1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8}),
				FNestedArray({1, 9, 4, 1, 4, 2, 2, 4, 6}),
				FNestedArray({8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1}),
				FNestedArray({10, 1, 0, 10, 0, 6, 6, 0, 4}),
				FNestedArray({4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3}),
				FNestedArray({10, 9, 4, 6, 10, 4}),
				FNestedArray({4, 9, 5, 7, 6, 11}),
				FNestedArray({0, 8, 3, 4, 9, 5, 11, 7, 6}),
				FNestedArray({5, 0, 1, 5, 4, 0, 7, 6, 11}),
				FNestedArray({11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5}),
				FNestedArray({9, 5, 4, 10, 1, 2, 7, 6, 11}),
				FNestedArray({6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5}),
				FNestedArray({7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2}),
				FNestedArray({3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6}),
				FNestedArray({7, 2, 3, 7, 6, 2, 5, 4, 9}),
				FNestedArray({9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7}),
				FNestedArray({3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0}),
				FNestedArray({6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8}),
				FNestedArray({9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7}),
				FNestedArray({1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4}),
				FNestedArray({4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10}),
				FNestedArray({7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10}),
				FNestedArray({6, 9, 5, 6, 11, 9, 11, 8, 9}),
				FNestedArray({3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5}),
				FNestedArray({0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11}),
				FNestedArray({6, 11, 3, 6, 3, 5, 5, 3, 1}),
				FNestedArray({1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6}),
				FNestedArray({0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10}),
				FNestedArray({11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5}),
				FNestedArray({6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3}),
				FNestedArray({5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2}),
				FNestedArray({9, 5, 6, 9, 6, 0, 0, 6, 2}),
				FNestedArray({1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8}),
				FNestedArray({1, 5, 6, 2, 1, 6}),
				FNestedArray({1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6}),
				FNestedArray({10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0}),
				FNestedArray({0, 3, 8, 5, 6, 10}),
				FNestedArray({10, 5, 6}),
				FNestedArray({11, 5, 10, 7, 5, 11}),
				FNestedArray({11, 5, 10, 11, 7, 5, 8, 3, 0}),
				FNestedArray({5, 11, 7, 5, 10, 11, 1, 9, 0}),
				FNestedArray({10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1}),
				FNestedArray({11, 1, 2, 11, 7, 1, 7, 5, 1}),
				FNestedArray({0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11}),
				FNestedArray({9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7}),
				FNestedArray({7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2}),
				FNestedArray({2, 5, 10, 2, 3, 5, 3, 7, 5}),
				FNestedArray({8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5}),
				FNestedArray({9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2}),
				FNestedArray({9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2}),
				FNestedArray({1, 3, 5, 3, 7, 5}),
				FNestedArray({0, 8, 7, 0, 7, 1, 1, 7, 5}),
				FNestedArray({9, 0, 3, 9, 3, 5, 5, 3, 7}),
				FNestedArray({9, 8, 7, 5, 9, 7}),
				FNestedArray({5, 8, 4, 5, 10, 8, 10, 11, 8}),
				FNestedArray({5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0}),
				FNestedArray({0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5}),
				FNestedArray({10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4}),
				FNestedArray({2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8}),
				FNestedArray({0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11}),
				FNestedArray({0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5}),
				FNestedArray({9, 4, 5, 2, 11, 3}),
				FNestedArray({2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4}),
				FNestedArray({5, 10, 2, 5, 2, 4, 4, 2, 0}),
				FNestedArray({3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9}),
				FNestedArray({5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2}),
				FNestedArray({8, 4, 5, 8, 5, 3, 3, 5, 1}),
				FNestedArray({0, 4, 5, 1, 0, 5}),
				FNestedArray({8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5}),
				FNestedArray({9, 4, 5}),
				FNestedArray({4, 11, 7, 4, 9, 11, 9, 10, 11}),
				FNestedArray({0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11}),
				FNestedArray({1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11}),
				FNestedArray({3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4}),
				FNestedArray({4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2}),
				FNestedArray({9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3}),
				FNestedArray({11, 7, 4, 11, 4, 2, 2, 4, 0}),
				FNestedArray({11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4}),
				FNestedArray({2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9}),
				FNestedArray({9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7}),
				FNestedArray({3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10}),
				FNestedArray({1, 10, 2, 8, 7, 4}),
				FNestedArray({4, 9, 1, 4, 1, 7, 7, 1, 3}),
				FNestedArray({4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1}),
				FNestedArray({4, 0, 3, 7, 4, 3}),
				FNestedArray({4, 8, 7}),
				FNestedArray({9, 10, 8, 10, 11, 8}),
				FNestedArray({3, 0, 9, 3, 9, 11, 11, 9, 10}),
				FNestedArray({0, 1, 10, 0, 10, 8, 8, 10, 11}),
				FNestedArray({3, 1, 10, 11, 3, 10}),
				FNestedArray({1, 2, 11, 1, 11, 9, 9, 11, 8}),
				FNestedArray({3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9}),
				FNestedArray({0, 2, 11, 8, 0, 11}),
				FNestedArray({3, 2, 11}),
				FNestedArray({2, 3, 8, 2, 8, 10, 10, 8, 9}),
				FNestedArray({9, 10, 2, 0, 9, 2}),
				FNestedArray({2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8}),
				FNestedArray({1, 10, 2}),
				FNestedArray({1, 3, 8, 9, 1, 8}),
				FNestedArray({0, 9, 1}),
				FNestedArray({0, 3, 8}),
				FNestedArray({-1, -1, -1})};


	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AVObject::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetOwner(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void AVObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVObject::SetParameters(float Resolution, FVector Origin, float Scale, float SurfaceLevel, int VGridID)
{
	CResolution = Resolution;
	COrigin = Origin;
	CScale = Scale;
	CSurfaceLevel = SurfaceLevel;
	GridID = VGridID;

	VoxelVertexData.Init(FVoxel(), CResolution*CResolution*CResolution);
}

void AVObject::CalcSurface()
{
	//Calculate current surface values using the default density (or other method) algorithm
	//Current approach. One day will try to reduce redundant calculations
	//Find a better way to do a bit array
	TArray<bool> PointCoverage = {false, false, false, false, false, false, false, false};
	for (int x = COrigin.X; x < (COrigin.X + CResolution); x++) {
		for (int y = COrigin.Y; y < (COrigin.Y + CResolution); y++) {
			for (int z = COrigin.Z; z < (COrigin.Z + CResolution); z++) {

				int AdjX = x * CScale;
				int AdjY = y * CScale;
				int AdjZ = z * CScale;

				PointCoverage[0] = PointCovered(AdjX, AdjY, AdjZ);
				PointCoverage[1] = PointCovered(AdjX + CScale, AdjY, AdjZ);
				PointCoverage[2] = PointCovered(AdjX + CScale, AdjY + CScale, AdjZ);
				PointCoverage[3] = PointCovered(AdjX, AdjY + CScale, AdjZ);
				PointCoverage[4] = PointCovered(AdjX, AdjY, AdjZ + CScale);
				PointCoverage[5] = PointCovered(AdjX + CScale, AdjY, AdjZ + CScale);
				PointCoverage[6] = PointCovered(AdjX + CScale, AdjY + CScale, AdjZ + CScale);
				PointCoverage[7] = PointCovered(AdjX, AdjY + CScale, AdjZ + CScale);

				int ci = 0;
				if (PointCoverage[0]) { ci += 1; }
				if (PointCoverage[1]) { ci += 2; }
				if (PointCoverage[2]) { ci += 4; }
				if (PointCoverage[3]) { ci += 8; }
				if (PointCoverage[4]) { ci += 16; }
				if (PointCoverage[5]) { ci += 32; }
				if (PointCoverage[6]) { ci += 64; }
				if (PointCoverage[7]) { ci += 128; }

				int index = getArrayIndex(x, y, z);

				VoxelVertexData[index].shape_index = ci;
				
				if (ci == 0) {
					VoxelVertexData[index].is_active = false;
				}
				else if (ci == 255) {
					VoxelVertexData[index].block_type = 2;
					VoxelVertexData[index].is_active = false;
				}
				else {
					VoxelVertexData[index].block_type = 1;
					VoxelVertexData[index].is_active = true;
				}

			}

		}

	}
}

void AVObject::GenerateSurface()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Generating Mesh Called")));

	int NumOfVertices = 0;

	TArray<int> Triangles;
	TArray<FVector> ShiftedVertices;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (int x = COrigin.X; x < (COrigin.X + CResolution); x++) {
		for (int y = COrigin.Y; y < (COrigin.Y + CResolution); y++) {
			for (int z = COrigin.Z; z < (COrigin.Z + CResolution); z++) {

				int index = getArrayIndex(x, y, z);
				int ci = VoxelVertexData[index].shape_index;
				if (VoxelVertexData[index].is_active) {
					TArray<FVector> triVertices = {};

					//Calculates vertex locations, triangles, and sums number of vertices
					for (int v : edgeMap[ci].TriangleList) {
						ShiftedVertices.Add((EdgeVertexPoint[v] + FVector(x, y, z))*CScale);
						triVertices.Add((EdgeVertexPoint[v] + FVector(x, y, z))*CScale);
						Triangles.Add(NumOfVertices);
						NumOfVertices++;
					}

					// adds normals, UV0, tangents, and colors
					int triCount = edgeMap[ci].TriangleList.Num() / 3;			
					for (int w = 0; w < triCount; w++) {
						normals.Add(CalcFaceNormal(triVertices[w], triVertices[w+1], triVertices[w+2]));
						UV0.Add(FVector2D(100, 0));
						tangents.Add(FProcMeshTangent(0, 1, 0));
						vertexColors.Add(getColor(VoxelVertexData[index].block_type));
					}
					
				}

			}
		}
	}

	

	//Will change this for materials
	Mesh->CreateMeshSection_LinearColor(0, ShiftedVertices, Triangles, normals, UV0, vertexColors, tangents, true);
	Mesh->ContainsPhysicsTriMeshData(true);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("X Y Z")));
}


FVector AVObject::GetVoxelSurfacePoint(float x, float y, float z, bool openPoint)
{
	// 1. Convert x,y,z to be relative to voxel surface

	FVector CurrentLocation = GetActorLocation();
	FVector HitLoc = (FVector(x, y, z) - CurrentLocation) / CScale;

	// 2. Use coords to get voxel

	int index = getArrayIndex(int(HitLoc.X), int(HitLoc.Y), int(HitLoc.Z));

	// 3. Select the closest active vertex coords
	// 3.1. Get voxel shape id
	int shape = VoxelVertexData[index].shape_index;
	int temp = shape;
	TArray<bool> coveredVertices = {false, false, false, false, false, false, false, false};
	for (int i = 7; i >= 0; i--) {
		// 3.2. Find all covered vertices
		int checkNum = FMath::Exp2(i);
		if (checkNum <= temp) {
			coveredVertices[i] = true;
			temp -= checkNum;
		}
		if (temp == 0) {
			break;
		}
	}
	// 3.3. Calc coords of each covered vertex
	TArray<FVector>vertexCoords = { FVector(int(HitLoc.X),		int(HitLoc.Y),		int(HitLoc.Z)),
									FVector(int(HitLoc.X) + 1,	int(HitLoc.Y),		int(HitLoc.Z)),
									FVector(int(HitLoc.X) + 1,	int(HitLoc.Y) + 1,	int(HitLoc.Z)),
									FVector(int(HitLoc.X),		int(HitLoc.Y) + 1,	int(HitLoc.Z)),
									FVector(int(HitLoc.X),		int(HitLoc.Y),		int(HitLoc.Z) + 1),
									FVector(int(HitLoc.X) + 1,	int(HitLoc.Y),		int(HitLoc.Z) + 1),
									FVector(int(HitLoc.X) + 1,	int(HitLoc.Y) + 1,	int(HitLoc.Z) + 1),
									FVector(int(HitLoc.X),		int(HitLoc.Y) + 1,	int(HitLoc.Z) + 1)};


	FVector closestPoint;
	float closestDist = FLT_MAX;
	for(int i = 0; i < 8; i++) {
		if (coveredVertices[i] && !openPoint) {
			int dist = FVector::Dist(HitLoc, vertexCoords[i]);
			if (dist < closestDist) {
				closestDist = dist;
				closestPoint = vertexCoords[i];
			}
		}
		else if (!coveredVertices[i] && openPoint){
			int dist = FVector::Dist(HitLoc, vertexCoords[i]);
			if (dist < closestDist) {
				closestDist = dist;
				closestPoint = vertexCoords[i];
			}
		}
	}
	
	return closestPoint;
	// 4. return the coords of the active vertext point
	
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("casted Hit Loc X Y Z: %d, %d, %d"), int(HitLoc.X), int(HitLoc.Y), int(HitLoc.Z)));
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("casted Hit Loc X Y Z: %d, %d, %d"), int(HitLoc.X), int(HitLoc.Y), int(HitLoc.Z)));
	

	//VoxelVertexData[index].shape_index = 0;

	
}

void AVObject::editVoxelVertices_Implementation(int x, int y, int z, uint8 edit, bool activate)
{

	int64 changeValue = FMath::Exp2(edit);
	int index = getArrayIndex(x, y, z);
	int64 ci = VoxelVertexData[index].shape_index;

	int64 test = UKismetMathLibrary::And_Int64Int64(changeValue, ci);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("changeValue: %d ci&changeValue: %d"),  changeValue, test));
	if (activate) {
		ci += changeValue;

		if (ci < 256) {
			VoxelVertexData[index].shape_index = ci;
			VoxelVertexData[index].is_active = !(ci == 255);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I should never seen this message")));
		}

	}
	else {
		ci -= changeValue;
		if (ci >= 0 && test == changeValue) {
			VoxelVertexData[index].shape_index = ci;
			VoxelVertexData[index].is_active = !(ci == 0);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I should never seen this message. Value was %d and trying to remove %d"), VoxelVertexData[index].shape_index, changeValue));
		}
	}


}

bool AVObject::editVoxelVertices_Validate(int x, int y, int z, uint8 edit, bool activate)
{
	return true;
}

FLinearColor AVObject::getColor(uint8 block_type)
{
	// Temporary solution to grab colors
	// Later will add material functionality and color mapping
	if (block_type == 1) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("good Color")));
		return FLinearColor(38, 186, 27, 1);
	}
	else if (block_type == 2) {
		return FLinearColor(84, 39, 1, 1);
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Bad Color")));
		return FLinearColor(185, 183, 182, 1);
	}
}

bool AVObject::PointCovered(float x, float y, float z)
{

	//float test = 70*FMath::PerlinNoise1D(x);

	//float val = (FMath::Sin(z)*FMath::Cos(x))+ (FMath::Sin(y)*FMath::Cos(z));
	float val = z + 70*(FMath::Sin(0.5*y) + FMath::Cos(0.5*x));
	return val <= CSurfaceLevel;

}

void AVObject::SingleVoxel_EditSurrounding(int x, int y, int z, uint8 bits)
{
	int index = getArrayIndex(x, y, z);

}

void AVObject::SingleVoxel_Isolated(int x, int y, int z, uint8 bits)
{
}

int AVObject::getArrayIndex(int x, int y, int z)
{
	
	int index = (x - COrigin.X)*CResolution*CResolution + (y - COrigin.Y)*CResolution + (z - COrigin.Z);

	return index;
}

FVector AVObject::CalcFaceNormal(FVector a, FVector b, FVector c)
{
	FVector U = FVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
	FVector V = FVector(c.X - b.X, c.Y - b.Y, c.Z - b.Z);

	return FVector::CrossProduct(U, V);
}

