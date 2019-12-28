// ================================================================================================
// .NET の真似系
// ================================================================================================
export type Action = <TArgs = any>(...args: TArgs[]) => void;
export type Func = <TR, TArgs = any>(...args: TArgs[]) => TR;

// ================================================================================================
// 基礎
// ================================================================================================
type T1 = 'a' | 'b' | 'c';

type Person = { name: string, age: number };
type PersonKeys = keyof Person;
type PersonAgeType = keyof Person['age'];

interface PropertyUtils<T> {
  getNameType<TKey extends keyof T>(key: TKey): TKey;
  getValueType<TKey extends keyof T>(key: TKey): T[TKey];
}

let pu: PropertyUtils<Person>;
const v2 = pu.getNameType('name');
const v3 = pu.getValueType('name');

type PersonEx1 = Person & { [TKey in 'tel' | 'mail' | 'addr']: string };
type PersonEx2 = { [TKey in 'name' | 'tel']: PersonEx1[TKey] }
type PersonEx3 = { [TKey in keyof PersonEx1]: number };
type PersonEx4 = { readonly [TKey in keyof PersonEx1]: PersonEx1[TKey] };

type Immutable<T> =
  T extends object
  ? { readonly [TKey in keyof T]: Immutable<T[TKey]> }
  : T
  ;
type Mutable<T> =
  T extends object
  ? { -readonly [TKey in keyof T]: Mutable<T[TKey]> }
  : T
  ;

type T2 = Immutable<number>;
type T3 = Mutable<string>;
type PersonEx5 = Immutable<PersonEx1>;
type PersonEx6 = Mutable<PersonEx4>;

// ================================================================================================
// 再実装してみる系
// ================================================================================================

type MyPartial<T, TRecursive extends true | false = false> =
  TRecursive extends true
  ? { [TKey in keyof T]?: MyPartial<T[TKey], TRecursive> }
  : { [TKey in keyof T]?: T[TKey] }
  ;
type MyRequired<T, TRecursive extends true | false = false> =
  TRecursive extends true
  ? { [TKey in keyof T]-?: MyRequired<T[TKey], TRecursive> }
  : { [TKey in keyof T]-?: T[TKey] }
  ;

type MyReadOnly<T, TRecursive extends true | false = false> =
  TRecursive extends true
  ? { readonly [TKey in keyof T]: MyReadOnly<T[TKey], TRecursive> }
  : { readonly [TKey in keyof T]: T[TKey] }
  ;

type MyPick<T, TKeys extends keyof T> = {
  [TKey in TKeys]: T[TKey]
};

type MyRecord<TKeys extends keyof any, T> = {
  [TKey in TKeys]: T
};

type MyExclude<T, TTarget> = T extends TTarget ? never : T;
type MyExtract<T, TTarget> = T extends TTarget ? T : never;

type MyOmit<T, TKey extends keyof T> = MyPick<T, MyExclude<keyof T, TKey>>;

type MyNonNullable<T> =
  T extends null | undefined
  ? MyExclude<T, null | undefined>
  : T;

type MyNonNullable2<T> = T extends null | undefined ? never : T;

// これだと型指定が邪魔
type MyReturnType<TResult = any, TArgs = any, T extends (...args: TArgs[]) => TResult = () => TResult> =
  T extends (...args: TArgs[]) => infer R
  ? R
  : TResult;

// これだと型指定が邪魔
type MyReturnType2<TResult = any, TArgs = any, T extends (...args: TArgs[]) => TResult = () => TResult> = TResult;

type MyReturnType3<T extends (...args: any) => any> =
  T extends (...args: any) => infer R
  ? R
  : any;

type NestedType1 = { id: number, name: string, addr: { country: string, zip: string, pref: string } };
type T4 = MyPartial<NestedType1>;
type T5 = MyPartial<NestedType1, true>;
type T6 = MyRequired<T5>;
type T7 = MyRequired<T5, true>;
type T8 = MyReadOnly<NestedType1>;
type T9 = MyReadOnly<NestedType1, true>;
type T10 = MyPick<PersonEx1, 'mail'>;
type T11 = MyRecord<'a' | 'b' | 'c', Person>;
type T12 = MyExclude<'a' | 'b' | 'c', 'a'>;
type T13 = MyExtract<'a' | 'b' | 'c', 'a'>;
type T14 = MyPick<PersonEx1, MyExclude<keyof PersonEx1, 'mail'>>;
type T15 = MyOmit<PersonEx1, 'mail'>;
type T16 = MyNonNullable<string | null | undefined>;
type T17 = MyNonNullable2<string | null | undefined>;
const v4 = (id: number) => { return id; };
type T18 = MyReturnType3<typeof v4>;

