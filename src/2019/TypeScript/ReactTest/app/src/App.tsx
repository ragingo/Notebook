import React from 'react';
import './App.css';
import { RootContainer } from './container/root/RootContainer';
import { HelloWorldContainer } from './container/hello/HelloWorldContainer';

const App: React.FC = () => {
    return (
        <div className="App">
            <RootContainer />
            <HelloWorldContainer />
        </div>
    );
}

export default App;
