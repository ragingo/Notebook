import React, { ErrorInfo } from 'react';

type TableProps = {
    items: any[]
}

type TableState = {
    headerCols: JSX.Element[]
    rows: JSX.Element[]
}

export default class Table extends React.Component<TableProps, TableState> {

    constructor(props: TableProps) {
        super(props);
        this.state = { headerCols: [], rows: [] };
    }

    shouldComponentUpdate() {
        console.log("Table#shouldComponentUpdate");
        console.log(this.props);
        if (this.props.items === null ||
            this.props.items.length === 0) {
            console.log('props.items is empty');
            return false;
        }
        return true;
    }

    componentWillUpdate() {
        console.log("Table#componentWillUpdate");

        this.state.headerCols.splice(0, this.state.headerCols.length);
        this.state.rows.splice(0, this.state.rows.length);

        let keys = Object.keys(this.props.items[0]);
        let headerCols: JSX.Element[] = [];
        keys.forEach((key, index) => {
            headerCols.push(
                <th key={index}>
                    {key}
                </th>
            );
        });

        let cols: JSX.Element[] = [];
        let rows: JSX.Element[] = [];

        this.props.items.forEach((item, index) => {
            keys.forEach((key, index) => {
                cols.push(
                    <td key={index}>
                        {item[key]}
                    </td>
                );
            });

            rows.push(
                <tr key={index}>
                    {cols}
                </tr>
            )

            cols = [];
        });

        headerCols.forEach(x => {
            this.state.headerCols.push(x);
        });
        rows.forEach(x => {
            this.state.rows.push(x);
        });
    }

    componentDidCatch(error: Error, errorInfo: ErrorInfo) {
        console.log("Table#componentDidCatch");
        console.log(error);
        console.log(errorInfo);
    }

    render() {
        console.log("Table#render");
        return (
            <table>
                <thead>
                    <tr>
                        {this.state.headerCols}
                    </tr>
                </thead>
                <tbody>
                    {this.state.rows}
                </tbody>
            </table>
        );
    }
}
