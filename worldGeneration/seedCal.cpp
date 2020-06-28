

/**
 * cover string to seed
 * 如果是 -9 到 9 之间的数字之间会随机生成一个种子
 * 在这个判定后会去除字符串两端的空白符号再做进一步处理
 * 如果是0或-0开头的数字当作字符串处理
 * 其它的按照普通数字处理(也就是两位数及以上的合法数字字符串)--直接当作种子
 * 字符串用了一个简单的哈希算法(和java 中的String.hashCode相同)
 * 
 * 
 * 生成后的种子会作为世界种子进行所有地形生成以及其它随机行为
 **/
#include <iostream>
int64_t parseSeedString(const std::string &str, int64_t _default) {
  if (str.length() < 2)return _default; //single char check
  if (str.length() == 2 && str[0] == '-')return _default;//check from -9 to -1
  // here need a trim() function and follow an empty check
  if (str[0] == '0' ||
      (str.length() > 1 && str[0] == '-' && str[1] == '0'))
    return _default; //start with 0 or -0
  try {
    return std::stoi(str);
  } catch (std::invalid_argument &e) {
    int hashCode = 0;
    for (auto c:str)
      hashCode = 31 * hashCode + c;
    return hashCode;
  }
}

int main() {
  std::cout << parseSeedString("abcdefg", 0);
  return 0;
}
