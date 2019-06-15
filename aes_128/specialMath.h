signed int bestModu(signed int n, size_t f){
        signed int ret = 0;
        if(f == 0){
                return -1;
        }

        if(n == 0){
        /*
         * returning 0 because that is what the
         * result would be!
         */
                return 0;

        }else if(n >= 1){
        /* calculate the working % relative to
         * positive numbers!
         */
                while(n >= 1){
                        ret++;                          /* increment ret*/

                        if(ret >= f || ret <= -1)       /* is ret outside of the finite field? -1 is bc hackers*/
                                ret = 0;                /* yes, set to 0 bc if ret >= f we land in a nonexisting "pie slice"*/
                                                        /* no, rets value is correct */
                        n--;                            /* decrement the amount of steps remaining */
                }
                return ret;                             /* return result when n is a positive number */
        }else{
        /* calculate the working % relative to
         * negative numbers!
         */
                while(n <= -1){
                        ret--;                          /* decrement ret */
                        if(ret <= -1 || ret >= f)       /* is ret negative? f is bc hackers */
                                ret = f-1;              /* yes, set ret to f-1 bc to the left of 0 is f-1, not -1(positve amt of "pie slices")*/
                                                        /* no, rets value is correct */
                        n++;                            /* decrement total amunt of required steps. */
                }
                return ret;
        }

        /* incase of undefined errror */
        ret = 1;
        return ret;
}

int rijnMod(int value){
        int ret = 0;
        const int rijnField = 283;
        int rijnField_;
        const int rijnDigits = 9;
        int tmp = value;
        int tmp_;
        int valueDigits = 0;
        signed int digitDiff;

        if(value <= 256 || value == 0){
                return value;
        }

        if(tmp < 0){
                tmp = (value & 0xf0)+(value & 0x0f);
        }

        tmp_ = tmp;
        while(tmp_ > 0){
                tmp_ = tmp_ / 2;
                valueDigits++;
        }

        digitDiff = valueDigits - rijnDigits;
        rijnField_ = rijnField << digitDiff;

        while(tmp > 256){
                tmp = tmp ^ rijnField_;
                if(tmp > 256){
                        tmp_ = tmp;
                        rijnField_ = rijnField;
                        valueDigits = 0;
                        while(tmp_ > 0){
                                tmp_ = tmp_ /  2;
                                valueDigits++;
                        }
                        digitDiff = valueDigits - rijnDigits;
                        rijnField_ = rijnField << digitDiff;
                }
        }
        ret = tmp;

        if(ret < 0){
                ret = (ret & 0xf0)+(ret & 0x0f);
        }
        return ret;
}

char galoisMultiply(const unsigned int x, char y){
        char ret;
        unsigned int pre_ret;
        const unsigned int shifter = 0x1;
        unsigned int tmp = x;
        unsigned int tmp_ = y;
        int polynomialDigits = 0;
        int polynomialDigits_valid = 0;

        if(y < 0){
                tmp_ = (y & 0xf0)+(y & 0x0f);
        }

        while(tmp > 0){
                tmp = tmp / 2;
                polynomialDigits++;
        }

        unsigned int poly[polynomialDigits];

        for(int i=0; i<polynomialDigits; i++){
                poly[i] = (x & (shifter<<i));
                if((x & (shifter<<i)) != 0){
                        polynomialDigits_valid++;
                }
        }

        unsigned int polynomial_array[polynomialDigits_valid];

        tmp = 0; /* Increments the above array */
        for(int i=0; i<polynomialDigits; i++){
                if(poly[i] != 0){
                        if(tmp < polynomialDigits_valid)
                                polynomial_array[tmp] = i;
                        tmp++;
                }
        }

        pre_ret = 0x0;
        for(int i=0; i<polynomialDigits_valid; i++){
                tmp = tmp_;
                tmp = tmp << polynomial_array[i];
                pre_ret = pre_ret ^ tmp;
        }

	ret = pre_ret;

        return ret;
}
