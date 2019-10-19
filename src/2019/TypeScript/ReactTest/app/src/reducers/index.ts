import { combineReducers} from 'redux';
import { testReducer, TestState } from './test';

export interface State {
    testReducer: TestState
}

export const reducers = combineReducers<State>({
    testReducer
});