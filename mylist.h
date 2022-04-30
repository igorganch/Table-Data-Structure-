
template <typename T>
class DList {
    struct Node {
        Node(const T& data = T{}, Node* next = nullptr, Node* prev = nullptr, std::string key = {}) {
            data_ = data;
            key_ = key; 
            prev_ = prev;
            next_ = next;
        }
        T data_;
        std::string key_; 
        Node* prev_;
        Node* next_;
    };
    Node* front_;
    Node* back_;
    int size_;

public:

    DList() {
        front_ = new Node();
        back_ = new Node();
        front_->next_ = back_;
        back_->prev_ = front_;
        size_ = 0;
    }
    class const_iterator {
        friend class DList;
    protected:
        Node* curr_;
    public:
        const_iterator(Node* n) {
            curr_ = n;
        }
        const_iterator() {
            curr_ = nullptr;
        }
        bool operator==(const_iterator rhs) {
            return curr_ == rhs.curr_;
        }
        bool operator!=(const_iterator rhs) {
            return curr_ != rhs.curr_;
        }
        const_iterator operator++() {
            if (curr_) {
                curr_ = curr_->next_;

            }
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator pre = *this;
            if (curr_) {
                curr_ = curr_->next_;
            }
            return pre;
        }
        const_iterator operator--() {
            if (curr_) {
                curr_ = curr_->prev_;
            }
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator pre = *this;
            if (curr_) {
                curr_ = curr_->prev_;
            }
            return pre;
        }
        const T& operator*() const {
            return curr_->data_;
        }
        const std::string& getKey() const {
            return this->curr_->key_;
        }
    };
    class iterator :public const_iterator {
    public:
        iterator() :const_iterator() {}
        iterator(Node* n) :const_iterator(n) {}
        T& operator*() {
            return this->curr_->data_;
        }
        std::string& getKey() {
            return this->curr_->key_;
        }
        iterator operator++() {
            if (this->curr_)
                this->curr_ = this->curr_->next_;
            return *this;
        }
        iterator operator++(int) {
            iterator ret = *this;
            if (this->curr_)
                this->curr_ = this->curr_->next_;
            return ret;
        }
        iterator operator--() {
            if (this->curr_)
                this->curr_ = this->curr_->prev_;
            return *this;
        }
        iterator operator--(int) {
            iterator ret = *this;
            if (this->curr_)
                this->curr_ = this->curr_->prev_;
            return ret;
        }
    };
    iterator begin() {
        return iterator(front_->next_);
    }
    iterator end() {
        return iterator(back_);
    }
    const_iterator cbegin() const {
        return const_iterator(front_->next_);
    }
    const_iterator cend() const {
        return const_iterator(back_);
    }
    void push_front(const T& data, std::string key) {

        Node* nf = new Node(data, front_->next_, front_,key);

        front_->next_->prev_ = nf;
        front_->next_ = nf;
        ++size_;
    }
    void push_back(const T& data, std::string key) {
        Node* nf = new Node(data, back_, back_->prev_, key);

        back_->prev_->next_ = nf;
        back_->prev_ = nf;
        ++size_;
    }
    void pop_front() {
        if (front_->next_ != back_) {
            Node* rm = front_->next_; // node to remove

            front_->next_ = front_->next_->next_;  // set front next

            front_->next_->prev_ = front_;  // set front prev
            delete rm;
            --size;
        }



    }
    void pop_back() {
        if (back_->prev_ != front_) {
            Node* rm = back_->prev_;
            back_->prev_ = back_->prev_->prev_;
            back_->prev_->next_ = back_;
            delete rm;

            --size;

        }




    }
    iterator erase(iterator it) {
        if (it.curr_ && it != end()) {
            Node* next = it.curr_->next_;
            it.curr_->next_->prev_ = it.curr_->prev_;
            it.curr_->prev_->next_ = it.curr_->next_;
            delete it.curr_;
            --size_;
            return next;
        }
    }

    iterator insert(iterator loc, const T& data) {
        if (loc.curr_) {                      //next          //last
            Node* temp = new  Node(data, loc.curr_, loc.curr_->prev_);
            loc.curr_->prev_->next_ = temp;
            loc.curr_->prev_ = temp;

            ++size_;
        }
        return --loc;
    }


    void sort(iterator f, iterator l) {


        iterator m;
        for (iterator ih = f; ih != l; ih++) {
            std::cout << *ih << std::endl;
        }
        std::cout << std::endl;

        for (iterator it = f; it != l; it++) {
            m = it;
            //    std::cout << it.curr_->data_ << std::endl;
            for (iterator ij = it; ij != l; ij++) {

                if (*ij < *m) {
                    m = ij;
                }
            }

            if (m != it) {

                if (m.curr_->prev_ == it.curr_) {
                    Node* temp_it = it.curr_;
                    Node* temp_m = m.curr_;
                    Node* it_prev = it.curr_->prev_;  //0
                    Node* it_next = it.curr_->next_;  //0
                    Node* m_next = m.curr_->next_; //01
                    it.curr_ = m.curr_;
                    m.curr_ = temp_it;

                    m.curr_->prev_ = it.curr_;
                    it.curr_->next_ = m.curr_;
                    it.curr_->prev_ = it_prev;
                    m.curr_->next_ = m_next;
                    it.curr_->prev_->next_ = it.curr_;
                    it.curr_->prev_->next_ = it.curr_;;
                    m.curr_->prev_->next_ = m.curr_;
                    m_next->prev_ = m.curr_;

//                    std::cout << "(What is f) - " << *f << " (IT)- " << *it << " (IT next)- " << it.curr_->next_->data_ << " (IT.prev->next) - " << it.curr_->prev_->next_->data_ << " (IT.next->prev-) " << it.curr_->next_->prev_->data_ << " (M)- " << *m << " (m->next) - " << m.curr_->next_->data_ << " (m->prev) - " << m.curr_->prev_->data_ << " (m.prev->next) - " << m.curr_->prev_->next_->data_ << " (m.next->prev) - " << m.curr_->next_->prev_->data_ << std::endl;


                }
                else {

                    //std::cout << *m << "!=" << *it << std::endl;
                    Node* temp_it = it.curr_;
                    Node* temp_m = m.curr_;
                    Node* it_prev = it.curr_->prev_;  //0
                    Node* it_next = it.curr_->next_;  //0

                    it.curr_ = m.curr_;
                    m.curr_ = temp_it;
                    //std::cout << "IT - " << *it << " M - " << *m << " Temp - " << temp_it->data_ << std::endl;

                    m.curr_->next_ = temp_m->next_;
                    temp_m->next_->prev_ = m.curr_;
                    m.curr_->prev_ = temp_m->prev_;
                    temp_m->prev_->next_ = m.curr_;
                    //std::cout << "IT next- " << it.curr_->next_->data_ << " M - " << *m << " Temp->next - " << temp_it->next_->data_ << " IT_PREV - " << it_prev->data_ << std::endl;

                    it.curr_->next_ = it_next;
                    it_next->prev_ = it.curr_;
                    it.curr_->prev_ = it_prev;
                    it_prev->next_ = it.curr_;
                    if (temp_it == f.curr_) {
                        f = it;

                    }

                   // std::cout << "(What is f) - " << *f << " (IT)- " << *it << " (IT next)- " << it.curr_->next_->data_ << " (IT.prev->next) - " << it.curr_->prev_->next_->data_ << " (IT.next->prev-) " << it.curr_->next_->prev_->data_ << " (M)- " << *m << " (m->next) - " << m.curr_->next_->data_ << " (m->prev) - " << m.curr_->prev_->data_ << std::endl;
                }

                for (iterator ih = f; ih != l; ih++) {
                    std::cout << *ih << std::endl;
                }

                /* m.curr_->next_->prev_ = m.curr_->prev_;
                 m.curr_->prev_->next_ = m.curr_->next_;
                 m.curr_->next_ = it.curr_;
                 m.curr_->prev_ = it.curr_->prev_->next_;
                 it.curr_->prev_->next_ = m.curr_;
                 it.curr_->prev_ = m.curr_;
                 std::cout << "IT - " << *it << std::endl;
                 */

                 /*
                 Node* temp_it = it.curr_->next_->prev_;
                 Node* temp_ = m.curr_;
                 Node* it_prev = it.curr_->prev_;  //0
                 Node* it_next = it.curr_->next_;  //0
                 Node* m_next = m.curr_->next_; //01
                 Node* m_prev = m.curr_->prev_; //01
                 it.curr_ = m.curr_;  // it is min
                 m.curr_ = temp_it; // min is it

                 it_prev->next_ = it.curr_;
                 it_next->prev_ = it.curr_;
                 it.curr_->next_ = it_next;
                 it.curr_->prev_ = it_prev;
                 m_prev->next_ = m.curr_;
                 m_next->prev_  = m.curr_;
                 m.curr_->next_ = m_next;
                 m.curr_->prev_ = m_prev;



                 Node* temp = it.curr_->prev_;  //0
                 Node* temp_next = it.curr_->next_; //01
                 it.curr_->next_ = m.curr_->next_;  //1
                 m.curr_->next_->prev_ = it.curr_; //2
                 it.curr_->prev_ = m.curr_->prev_;  //3
                 m.curr_->prev_->next_ = it.curr_; //4
                 temp->next_ = m.curr_; // 5
                 m.curr_->prev_ = temp; // 6
                 temp_next->prev_ = m.curr_; // 7
                 m.curr_->next_ = temp_next;
                 break;

               Node* calvin = min.curr_;
                 Node* the = it.curr_;
                 Node* add = calvin->next_;
                 Node* dog = calvin->prev_;
                 Node* quick = the->next_;
                 Node* front = the->prev_;


                 front->next_ = calvin;
                 calvin->prev_ = front;
                 calvin->next_ = quick;
                 quick->prev_ = calvin;
                 dog->next_ = the;
                 add->prev_ = the;
                 the->prev_ = dog;
                 the->next_ = add;
               */
                std::cout << std::endl;











            }

        }






    }

    iterator searchKey(const std::string key) {
        for (iterator it = begin(); it != end(); it++) {
            if (it.curr_->key_ == key) {
                return it;
            }
        }
        return end();
    }

    iterator search(const T& data) {
        for (iterator it = begin(); it != end(); it++) {
            if (*it == data) {
                return it;
            }
        }
        return end();
    }
    const_iterator search(const T& data) const {
        for (const_iterator it = begin(); it != end(); it++) {
            if (*it == data) {
                return it;
            }
        }
        return end();
    }
    bool empty() const {
        return front_->next_ == back_;
    }
    int size() const {
        return size_;
    }
    ~DList() {

        delete front_;
        delete back_;

    }
    //Copy constuctor 
    DList(const DList& obj) {
        front_ = new Node();
        back_ = new Node();
        front_->next_ = back_;
        back_->prev_ = front_;
        size_ = 0;;
        for (const_iterator itor = obj.cbegin(); itor != obj.cend(); itor++) {
            push_back(*itor);
        }
    }
    //Copy Assignment
    DList& operator=(const DList& obj) {
        iterator f = begin();
        iterator l = end();

        Node* prev = f.curr_->prev_;

        while (f != l && f != end()) {
            ++f;
            delete f.curr_->prev_;
            --size_;
        }
        Node* next = (f == l ? l.curr_ : back_);
        prev->next_ = next;
        next->prev_ = prev;


        for (const_iterator it = obj.cbegin(); it != obj.cend(); it++) {
            push_back(*it, it.curr_->key_);
        }
        return *this;
    }
    //Move contructor
    DList(DList&& obj) {
        if (this != &obj) {

            front_ = new Node();
            back_ = new Node();
            front_->next_ = back_;
            back_->prev_ = front_;
            size_ = 0;

            for (const_iterator it = obj.cbegin(); it != obj.cend(); it++) {
                push_back(*it);
            }
            obj.front_->next_ = nullptr;
            obj.back_->prev_ = nullptr;

        }
    }
    //Move assignemnt
    DList& operator=(DList&& obj) {
        iterator f = begin();
        iterator l = end();
        Node* prev = f.curr_->prev_;
        while (f != l && f != end()) {
            ++f;
            delete f.curr_->prev_;
            --size_;
        }
        Node* next = (f == l ? l.curr_ : back_);
        prev->next_ = next;
        next->prev_ = prev;

        for (const_iterator itor = obj.cbegin(); itor != obj.cend(); itor++) {
            push_back(*itor);
        }
        obj.front_->next_ = nullptr;
        obj.back_->prev_ = nullptr;
        return *this;


    }
    //
    //
};