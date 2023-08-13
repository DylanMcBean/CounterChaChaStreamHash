## README

### Counter ChaCha Stream Hash (CCSH)

The Counter ChaCha Stream Hash (CCSH) is a hashing mechanism inspired by the [ChaCha stream cipher](https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant). This library provides a simple API to compute hash values based on the ChaCha algorithm.

---

### Installation

Clone this repository:

```bash
git clone [repository-url]
```

Include the `ccsh.h` and the required dependencies in your project.

---

### API Usage

1. **Initialization:**

   Create an instance of the `CCSH` class.

   ```cpp
   CCSH hash;
   ```

2. **Starting the Hashing Process:**

   You can begin hashing data using either a string or a byte array.

   - **String Input:**

     ```cpp
     std::string data = "Hello, World!";
     hash.start(data);
     ```

   - **Byte Array Input:**

     ```cpp
     const uint8_t byteData[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};  // "Hello"
     hash.start(byteData, sizeof(byteData));
     ```

3. **Updating the Hash with Additional Data:**

   Similar to starting the hashing process, you can update the hash with more data using either a string or a byte array.

   - **String Input:**

     ```cpp
     std::string moreData = "!";
     hash.update(moreData);
     ```

   - **Byte Array Input:**

     ```cpp
     const uint8_t moreByteData[] = {0x21};  // "!"
     hash.update(moreByteData, sizeof(moreByteData));
     ```

4. **Retrieve the Hash:**

   Once you've hashed all your data, you can retrieve the hash value as a hexadecimal string.

   ```cpp
   std::string hashValue = hash.hexdump();
   std::cout << "Hash Value: " << hashValue << std::endl;
   ```

---

### Example

Here's a simple example that uses the `CCSH` API to hash a string.

```cpp
#include "ccsh.h"

int main() {
    CCSH hash;
    
    std::string data = "Hello, World!";
    hash.start(data);
    
    std::string hashValue = hash.hexdump();
    std::cout << "Hash Value: " << hashValue << std::endl;

    return 0;
}
```

---

### Notes

- This library is a simple implementation and may not be suitable for cryptographic purposes.
- The `CCSH` class maintains its internal state. If you want to hash another set of data, you need to reinitialize the hash object.

---

### Contributions

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

---

### License

[MIT](https://choosealicense.com/licenses/mit/)
