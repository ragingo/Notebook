
class TestClass1 {

    async downloadText(source: string): Promise<string> {
        var res = await fetch(source);
        if (!res.ok) {
          return Promise.reject(new Error("request failed."));
        }
        return Promise.resolve(await res.text());
    }
    
    async downloadJson(source: string): Promise<any> {
        var res: Response;
        try {
            res = await fetch(source, {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json'
                }
            });
            console.log(res);
        }
        catch (e) {
            return Promise.reject(e);
        }
        if (!res.ok) {
          return Promise.reject(new Error("request failed."));
        }
        return Promise.resolve(await res.json());
    }

    method1() {
        type Person = {
            age: number,
            name: string
        }
        var obj = {
            age: 20,
            name: 'ragingo1'
        } as Person;
        // type PersonKeys = keyof Person;
        Object.keys(obj).map(x => 'prop: ' + x).forEach(x => console.log(x));
    }

    async method2() {
        type Person = {
            age: number,
            name: string,
        }
        var json = await this.downloadJson('./data/data1.json');
        var person = json as Person;
        console.log(json);
        console.log(person);
    }
}


export default TestClass1;
