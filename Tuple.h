#pragma once
template<typename T1>
class Tuple1
{
public:
    T1 Item1;
public:
    Tuple1(T1 v1);
    Tuple1() {}
};
template<typename T1, typename T2>
class Tuple2
{
public:
    T1 Item1;
    T2 Item2;
public:
    Tuple2(T1 v1, T2 v2);
    Tuple2() {}
};
template<typename T1, typename T2, typename T3>
class Tuple3
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
public:
    Tuple3(T1 v1, T2 v2, T3 v3);
    Tuple3() {}
};
template<typename T1, typename T2, typename T3, typename T4>
class Tuple4
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
public:
    Tuple4(T1 v1, T2 v2, T3 v3, T4 v4);
    Tuple4() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5>
class Tuple5
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
public:
    Tuple5(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5);
    Tuple5() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Tuple6
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
    T6 Item6;
public:
    Tuple6(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6);
    Tuple6() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Tuple7
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
    T6 Item6;
    T7 Item7;
public:
    Tuple7(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7);
    Tuple7() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Tuple8
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
    T6 Item6;
    T7 Item7;
    T8 Item8;
public:
    Tuple8(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8);
    Tuple8() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Tuple9
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
    T6 Item6;
    T7 Item7;
    T8 Item8;
    T9 Item9;
public:
    Tuple9(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9);
    Tuple9() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Tuple10
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
    T6 Item6;
    T7 Item7;
    T8 Item8;
    T9 Item9;
    T10 Item10;
public:
    Tuple10(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10);
    Tuple10() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Tuple11
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
    T6 Item6;
    T7 Item7;
    T8 Item8;
    T9 Item9;
    T10 Item10;
    T11 Item11;
public:
    Tuple11(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11);
    Tuple11() {}
};
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Tuple12
{
public:
    T1 Item1;
    T2 Item2;
    T3 Item3;
    T4 Item4;
    T5 Item5;
    T6 Item6;
    T7 Item7;
    T8 Item8;
    T9 Item9;
    T10 Item10;
    T11 Item11;
    T12 Item12;
public:
    Tuple12(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12);
    Tuple12() {}
};



template<typename T1>
Tuple1<T1>::Tuple1(T1 v1)
{
    this->Item1 = v1;
}
template<typename T1, typename T2>
Tuple2<T1, T2>::Tuple2(T1 v1, T2 v2)
{
    this->Item1 = v1;
    this->Item2 = v2;
}
template<typename T1, typename T2, typename T3>
Tuple3<T1, T2, T3>::Tuple3(T1 v1, T2 v2, T3 v3)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
}
template<typename T1, typename T2, typename T3, typename T4>
Tuple4<T1, T2, T3, T4>::Tuple4(T1 v1, T2 v2, T3 v3, T4 v4)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
Tuple5<T1, T2, T3, T4, T5>::Tuple5(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Tuple6<T1, T2, T3, T4, T5, T6>::Tuple6(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
    this->Item6 = v6;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Tuple7<T1, T2, T3, T4, T5, T6, T7>::Tuple7(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
    this->Item6 = v6;
    this->Item7 = v7;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
Tuple8<T1, T2, T3, T4, T5, T6, T7, T8>::Tuple8(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
    this->Item6 = v6;
    this->Item7 = v7;
    this->Item8 = v8;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
Tuple9<T1, T2, T3, T4, T5, T6, T7, T8, T9>::Tuple9(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
    this->Item6 = v6;
    this->Item7 = v7;
    this->Item8 = v8;
    this->Item9 = v9;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
Tuple10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::Tuple10(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
    this->Item6 = v6;
    this->Item7 = v7;
    this->Item8 = v8;
    this->Item9 = v9;
    this->Item10 = v10;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
Tuple11<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::Tuple11(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
    this->Item6 = v6;
    this->Item7 = v7;
    this->Item8 = v8;
    this->Item9 = v9;
    this->Item10 = v10;
    this->Item11 = v11;
}
template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
Tuple12<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::Tuple12(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12)
{
    this->Item1 = v1;
    this->Item2 = v2;
    this->Item3 = v3;
    this->Item4 = v4;
    this->Item5 = v5;
    this->Item6 = v6;
    this->Item7 = v7;
    this->Item8 = v8;
    this->Item9 = v9;
    this->Item10 = v10;
    this->Item11 = v11;
    this->Item12 = v12;
}

