//
// Created by 郑轩 on 2023/9/14.
//

#include "pb.pb.h"
#include <google/protobuf/util/json_util.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <gtest/gtest.h>
#include <rapidjson/document.h>

template <class _Rep, class _Period>
void printTime(std::ostream& out, const char* prefix, const std::chrono::duration<_Rep, _Period>& d) {
  using namespace std::literals;
  out << prefix << std::setprecision(3);
  if (d / 1s > 0) {
    out << d * 1.0 / 1s << "s.";
  } else if (d / 1ms > 0) {
    out << d * 1.0 / 1ms << "ms.";
  } else {
    out << std::chrono::duration_cast<std::chrono::microseconds>(d).count() << "µs.";
  }

  out << std::endl;
}

class GenerateFile : public testing::Environment {
  void SetUp() override {
    using namespace std::literals;
    std::ofstream of("test.json");
    A a;
    char buf[1024];
    for(auto i=0; i<1000*1000; i++) {
      Status n;
      snprintf(buf, sizeof(buf), "/Users/zxfishhack/Works/Private/jsonpb/%d", i);
      n.set_path(buf);
      n.set_createtime("Testing started at 12:22");
      a.mutable_b()->Add(std::move(n));
      n.set_path(buf);
      n.set_createtime("Testing started at 12:22");
      a.mutable_c()->Add(std::move(n));
      n.set_path(buf);
      n.set_createtime("Testing started at 12:22");
      a.mutable_d()->Add(std::move(n));
      n.set_path(buf);
      n.set_createtime("Testing started at 12:22");
      a.mutable_e()->Add(std::move(n));
    }
    std::string json;
    google::protobuf::util::JsonOptions options;
    auto start = std::chrono::high_resolution_clock::now();
    auto st = google::protobuf::util::MessageToJsonString(a, &json, options);
    auto end = std::chrono::high_resolution_clock::now();
    EXPECT_TRUE(st.ok());
    of.write(json.c_str(), json.length());
    of.close();
    std::cout << "generate test file done" << std::endl;
    std::cout << "JSON file size: " << json.length() << "B ≈ " << json.length() / 1024 / 1024 << "MB" << std::endl;
    std::cout << "marshal protobuf to JSON cost "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "µs ≈ "
              << (end - start) / 1ms << "ms ≈ "
              << (end - start) / 1s << "s." << std::endl;
  }
};

const auto env = testing::AddGlobalTestEnvironment(new GenerateFile);

TEST(JSONPB, ProtobufJSON) {
  A a;
  std::ifstream input("test.json");
  input.seekg(0, std::ios_base::end);
  auto length = input.tellg();
  input.seekg(0, std::ios_base::beg);
  auto buf = new char[length];
  input.read(buf, length);
  auto start = std::chrono::high_resolution_clock::now();
  auto st = google::protobuf::util::JsonStringToMessage(buf, &a);
  auto end = std::chrono::high_resolution_clock::now();
  EXPECT_TRUE(st.ok());
  printTime(std::cout, "parse JSON to protobuf cost ", end - start);
}

TEST(JSONPB, RapidJSON) {
  A a;
  std::ifstream input("test.json");
  input.seekg(0, std::ios_base::end);
  auto length = input.tellg();
  input.seekg(0, std::ios_base::beg);
  auto buf = new char[length];
  input.read(buf, length);
  auto start = std::chrono::high_resolution_clock::now();
  rapidjson::Document doc;
  EXPECT_FALSE(doc.Parse(buf, length).HasParseError());
  auto mid = std::chrono::high_resolution_clock::now();
  for(auto s = doc["B"].Begin(); s != doc["B"].End(); s++) {
    Status status;
    status.set_createtime((*s)["CreateTime"].GetString());
    status.set_path((*s)["Path"].GetString());
    a.mutable_b()->Add(std::move(status));
  }
  for(auto s = doc["C"].Begin(); s != doc["C"].End(); s++) {
    Status status;
    status.set_createtime((*s)["CreateTime"].GetString());
    status.set_path((*s)["Path"].GetString());
    a.mutable_c()->Add(std::move(status));
  }
  for(auto s = doc["D"].Begin(); s != doc["D"].End(); s++) {
    Status status;
    status.set_createtime((*s)["CreateTime"].GetString());
    status.set_path((*s)["Path"].GetString());
    a.mutable_d()->Add(std::move(status));
  }
  for(auto s = doc["E"].Begin(); s != doc["E"].End(); s++) {
    Status status;
    status.set_createtime((*s)["CreateTime"].GetString());
    status.set_path((*s)["Path"].GetString());
    a.mutable_e()->Add(std::move(status));
  }
  auto end = std::chrono::high_resolution_clock::now();
  printTime(std::cout, "parse JSON cost ", mid - start);
  printTime(std::cout, "parse JSON to protobuf cost ", end - start);
}
