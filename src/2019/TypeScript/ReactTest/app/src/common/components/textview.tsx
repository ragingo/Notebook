import React from 'react';
import TestClass1 from '../Test';

type TextViewProp = {
    source: string
}

type TextViewState = {
    content: string
}

export default class TextView extends React.Component<TextViewProp, TextViewState> {

    constructor(props: TextViewProp) {
        super(props);
        this.state = { content: '' };
    }

    async componentWillMount() {
        let a = new TestClass1();
        this.setState({ content: await a.downloadText('./index.html') });
        return Promise.resolve();
    }

    render() {
        return (
            <div>
                {this.state.content}
            </div>
        );
    }
}
