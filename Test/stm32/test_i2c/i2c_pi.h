#ifndef TEST_I2C_h
#define TEST_I2C_h

class I2CMaster {
    public:
        I2CMaster(const char* bus = "/dev/i2c-1"){
            fd = open(bus, O_RDWR);
            if (fd < 0) throw std::runtime_error("I2C bus niet gevonden");
        }
    
        ~I2CMaster(){ if (fd >= 0) close(fd);
        }
    
        void setSlaveadd(uint8_t addr){
            if (ioctl(fd, I2C_SLAVE, addr) < 0) throw std::runtime_error("I2C adress niet geregistreerd");
        }
    
        void writeData(const std::vector<uint8_t>& data){
            if (::write(fd, data.data(), data.size()) != (ssize_t)data.size())
                throw std::runtime_error("I2C write error");
        }
    
        std::vector<uint8_t> readData(size_t len){
            std::vector<uint8_t> buf(len);
            if (::read(fd, buf.data(), len) != (ssize_t)len)
                throw std::runtime_error("I2C read error");
            return buf;
        }
    
    private:
        int fd;
    };

#endif // TEST_I2C_h