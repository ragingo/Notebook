
export interface TestState {
    name: string
}

const defaultTestState: () => TestState = () => ({name: "ragingo1"});

export const testReducer = (state = defaultTestState(), /*action*/) => {
    return state;
}
