import React, { ErrorInfo } from 'react';
import Table from '../../common/components/table';
import TestClass1 from '../../common/Test';
import './SearchContainer.css'

type RowDataFormat = {
    watchButton: JSX.Element
    contentId: string
    title: string
    viewCounter: number
    thumbnail: JSX.Element
}

type SearchContainerState = {
    keyword: string
    items: RowDataFormat[]
}

// 検索コンテナ
export class SearchContainer extends React.Component<{}, SearchContainerState> {

    private keyword: string = '';

    constructor(props: any) {
        super(props);
        this.state = { keyword: '', items: [] };

        this.onSearchButtonClick = this.onSearchButtonClick.bind(this);
        this.onSearchKeywordChanged = this.onSearchKeywordChanged.bind(this);
        this.onWatchButtonClicked = this.onWatchButtonClicked.bind(this);
    }

    // キーワード検索
    private async searchByKeyword(keyword: string) {
        console.log("SearchContainer#searchByKeyword");

        let a = new TestClass1();
        let url = `https://api.search.nicovideo.jp/api/v2/video/contents/search?q=${keyword}&targets=title&fields=contentId,title,viewCounter&_sort=-viewCounter&_offset=0&_limit=10&_context=apiguide`;
        try {
            let json = await a.downloadJson(url);
            console.log(json);

            this.state.items.splice(0, this.state.items.length);

            let data = json.data as Array<RowDataFormat>;
            data.forEach(item => {
                let id = item.contentId.substr(2);
                item.thumbnail = <img src={`https://tn.smilevideo.jp/smile?i=${id}.L`} />;
                item.watchButton = <button onClick={this.onWatchButtonClicked} value={item.contentId}>watch</button>
                this.state.items.push(item);
            });
        }
        catch (e) {
            console.log(e);
        }

        await Promise.resolve();
    }

    async componentDidMount() {
        console.log("SearchContainer#componentDidMount");
    }

    componentDidCatch(error: Error, errorInfo: ErrorInfo) {
        console.log("SearchContainer#componentDidCatch");
        console.log(error);
        console.log(errorInfo);
    }

    // 検索ボタンクリック
    async onSearchButtonClick(e: React.MouseEvent<HTMLButtonElement, MouseEvent>) {
        console.log("SearchContainer#onSearchButtonClick");
        e.preventDefault();

        await this.searchByKeyword(this.keyword);

        this.forceUpdate();
    }

    // 入力キーワード変更後
    onSearchKeywordChanged(e: React.ChangeEvent<HTMLInputElement>) {
        console.log("SearchContainer#onSearchKeywordChanged");
        console.log(e.target.value);
        this.keyword = e.target.value;
    }

    // 視聴ボタンクリック
    onWatchButtonClicked(e: React.MouseEvent<HTMLButtonElement, MouseEvent>) {
        console.log("SearchContainer#onSearchButtonClick");
        e.preventDefault();
        console.log(e.currentTarget.value);
        let contentId = e.currentTarget.value;
        window.open(`http://www.nicovideo.jp/watch/${contentId}`);
    }

    render() {
        console.log("SearchContainer#render");
        console.log(this.state);

        return (
            <div id="search_area">
                <input id={"keyword"} type='text' placeholder={"キーワードを入力してください"} onChange={this.onSearchKeywordChanged} />
                <button onClick={this.onSearchButtonClick}>検索</button>
                <Table items={this.state.items} />
            </div>
        );

    }

}
