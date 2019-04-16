#pragma once


namespace CodeList
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::Button^  button_0;

	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Button^  button_1;

	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::DataGridView^  dataGridView2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::Button^  button_2;

	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::DataGridView^  dataGridView3;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::Button^  button_3;

	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::DataGridView^  dataGridView4;
	private: System::Windows::Forms::TabPage^  tabPage6;
	private: System::Windows::Forms::Button^  button_5;

	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::DataGridView^  dataGridView5;
	private: System::Windows::Forms::TabPage^  tabPage7;
	private: System::Windows::Forms::Button^  button_6;

	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::DataGridView^  dataGridView6;
	private: System::Windows::Forms::TabPage^  tabPage8;
	private: System::Windows::Forms::Button^  button_7;

	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::DataGridView^  dataGridView7;
	private: System::Windows::Forms::TabPage^  tabPage9;
	private: System::Windows::Forms::Button^  button_8;

	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::DataGridView^  dataGridView8;
	private: System::Windows::Forms::TabPage^  tabPage10;
	private: System::Windows::Forms::Button^  button_9;

	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::DataGridView^  dataGridView9;
	private: System::Windows::Forms::TextBox^  textBox10;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  statusLabel1;

	private: System::Windows::Forms::ToolStripProgressBar^  progressBar1;


	protected: 






	protected: 

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->button_0 = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->button_1 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->button_2 = (gcnew System::Windows::Forms::Button());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView3 = (gcnew System::Windows::Forms::DataGridView());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->button_3 = (gcnew System::Windows::Forms::Button());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView4 = (gcnew System::Windows::Forms::DataGridView());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->button_5 = (gcnew System::Windows::Forms::Button());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView5 = (gcnew System::Windows::Forms::DataGridView());
			this->tabPage7 = (gcnew System::Windows::Forms::TabPage());
			this->button_6 = (gcnew System::Windows::Forms::Button());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView6 = (gcnew System::Windows::Forms::DataGridView());
			this->tabPage8 = (gcnew System::Windows::Forms::TabPage());
			this->button_7 = (gcnew System::Windows::Forms::Button());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView7 = (gcnew System::Windows::Forms::DataGridView());
			this->tabPage9 = (gcnew System::Windows::Forms::TabPage());
			this->button_8 = (gcnew System::Windows::Forms::Button());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView8 = (gcnew System::Windows::Forms::DataGridView());
			this->tabPage10 = (gcnew System::Windows::Forms::TabPage());
			this->button_9 = (gcnew System::Windows::Forms::Button());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->dataGridView9 = (gcnew System::Windows::Forms::DataGridView());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->progressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->statusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->BeginInit();
			this->tabPage4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView4))->BeginInit();
			this->tabPage6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView5))->BeginInit();
			this->tabPage7->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView6))->BeginInit();
			this->tabPage8->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView7))->BeginInit();
			this->tabPage9->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView8))->BeginInit();
			this->tabPage10->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView9))->BeginInit();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Controls->Add(this->tabPage6);
			this->tabControl1->Controls->Add(this->tabPage7);
			this->tabControl1->Controls->Add(this->tabPage8);
			this->tabControl1->Controls->Add(this->tabPage9);
			this->tabControl1->Controls->Add(this->tabPage10);
			this->tabControl1->Location = System::Drawing::Point(2, 2);
			this->tabControl1->Margin = System::Windows::Forms::Padding(0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(434, 336);
			this->tabControl1->TabIndex = 1;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->button_0);
			this->tabPage1->Controls->Add(this->dataGridView1);
			this->tabPage1->Controls->Add(this->textBox1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(426, 310);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"\\0";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// button_0
			// 
			this->button_0->Location = System::Drawing::Point(349, 1);
			this->button_0->Name = L"button_0";
			this->button_0->Size = System::Drawing::Size(75, 23);
			this->button_0->TabIndex = 2;
			this->button_0->Text = L"検索";
			this->button_0->UseVisualStyleBackColor = true;
			this->button_0->Click += gcnew System::EventHandler(this, &Form1::button_0_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Location = System::Drawing::Point(3, 25);
			this->dataGridView1->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowTemplate->Height = 21;
			this->dataGridView1->Size = System::Drawing::Size(420, 283);
			this->dataGridView1->TabIndex = 1;
			// 
			// textBox1
			// 
			this->textBox1->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox1->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox1->Location = System::Drawing::Point(3, 3);
			this->textBox1->Margin = System::Windows::Forms::Padding(0);
			this->textBox1->MaxLength = 100;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(343, 19);
			this->textBox1->TabIndex = 0;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->button_1);
			this->tabPage2->Controls->Add(this->textBox2);
			this->tabPage2->Controls->Add(this->dataGridView2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(426, 310);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"\\1";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// button_1
			// 
			this->button_1->Location = System::Drawing::Point(349, 1);
			this->button_1->Name = L"button_1";
			this->button_1->Size = System::Drawing::Size(75, 23);
			this->button_1->TabIndex = 4;
			this->button_1->Text = L"検索";
			this->button_1->UseVisualStyleBackColor = true;
			this->button_1->Click += gcnew System::EventHandler(this, &Form1::button_1_Click);
			// 
			// textBox2
			// 
			this->textBox2->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox2->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox2->Location = System::Drawing::Point(3, 3);
			this->textBox2->Margin = System::Windows::Forms::Padding(0);
			this->textBox2->MaxLength = 100;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(343, 19);
			this->textBox2->TabIndex = 3;
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->AllowUserToDeleteRows = false;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Location = System::Drawing::Point(3, 25);
			this->dataGridView2->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->ReadOnly = true;
			this->dataGridView2->RowTemplate->Height = 21;
			this->dataGridView2->Size = System::Drawing::Size(420, 283);
			this->dataGridView2->TabIndex = 2;
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->button_2);
			this->tabPage3->Controls->Add(this->textBox3);
			this->tabPage3->Controls->Add(this->dataGridView3);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(426, 310);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"\\2";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// button_2
			// 
			this->button_2->Location = System::Drawing::Point(349, 1);
			this->button_2->Name = L"button_2";
			this->button_2->Size = System::Drawing::Size(75, 23);
			this->button_2->TabIndex = 4;
			this->button_2->Text = L"検索";
			this->button_2->UseVisualStyleBackColor = true;
			this->button_2->Click += gcnew System::EventHandler(this, &Form1::button_2_Click);
			// 
			// textBox3
			// 
			this->textBox3->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox3->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox3->Location = System::Drawing::Point(3, 3);
			this->textBox3->Margin = System::Windows::Forms::Padding(0);
			this->textBox3->MaxLength = 100;
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(343, 19);
			this->textBox3->TabIndex = 3;
			// 
			// dataGridView3
			// 
			this->dataGridView3->AllowUserToAddRows = false;
			this->dataGridView3->AllowUserToDeleteRows = false;
			this->dataGridView3->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView3->Location = System::Drawing::Point(3, 25);
			this->dataGridView3->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView3->Name = L"dataGridView3";
			this->dataGridView3->ReadOnly = true;
			this->dataGridView3->RowTemplate->Height = 21;
			this->dataGridView3->Size = System::Drawing::Size(420, 283);
			this->dataGridView3->TabIndex = 2;
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->button_3);
			this->tabPage4->Controls->Add(this->textBox4);
			this->tabPage4->Controls->Add(this->dataGridView4);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(426, 310);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"\\3";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// button_3
			// 
			this->button_3->Location = System::Drawing::Point(349, 1);
			this->button_3->Name = L"button_3";
			this->button_3->Size = System::Drawing::Size(75, 23);
			this->button_3->TabIndex = 4;
			this->button_3->Text = L"検索";
			this->button_3->UseVisualStyleBackColor = true;
			this->button_3->Click += gcnew System::EventHandler(this, &Form1::button_3_Click);
			// 
			// textBox4
			// 
			this->textBox4->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox4->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox4->Location = System::Drawing::Point(3, 3);
			this->textBox4->Margin = System::Windows::Forms::Padding(0);
			this->textBox4->MaxLength = 100;
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(343, 19);
			this->textBox4->TabIndex = 3;
			// 
			// dataGridView4
			// 
			this->dataGridView4->AllowUserToAddRows = false;
			this->dataGridView4->AllowUserToDeleteRows = false;
			this->dataGridView4->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView4->Location = System::Drawing::Point(3, 25);
			this->dataGridView4->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView4->Name = L"dataGridView4";
			this->dataGridView4->ReadOnly = true;
			this->dataGridView4->RowTemplate->Height = 21;
			this->dataGridView4->Size = System::Drawing::Size(420, 283);
			this->dataGridView4->TabIndex = 2;
			// 
			// tabPage6
			// 
			this->tabPage6->Controls->Add(this->button_5);
			this->tabPage6->Controls->Add(this->textBox5);
			this->tabPage6->Controls->Add(this->dataGridView5);
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Padding = System::Windows::Forms::Padding(3);
			this->tabPage6->Size = System::Drawing::Size(426, 310);
			this->tabPage6->TabIndex = 5;
			this->tabPage6->Text = L"\\5";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// button_5
			// 
			this->button_5->Location = System::Drawing::Point(349, 1);
			this->button_5->Name = L"button_5";
			this->button_5->Size = System::Drawing::Size(75, 23);
			this->button_5->TabIndex = 4;
			this->button_5->Text = L"検索";
			this->button_5->UseVisualStyleBackColor = true;
			this->button_5->Click += gcnew System::EventHandler(this, &Form1::button_5_Click);
			// 
			// textBox5
			// 
			this->textBox5->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox5->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox5->Location = System::Drawing::Point(3, 3);
			this->textBox5->Margin = System::Windows::Forms::Padding(0);
			this->textBox5->MaxLength = 100;
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(343, 19);
			this->textBox5->TabIndex = 3;
			// 
			// dataGridView5
			// 
			this->dataGridView5->AllowUserToAddRows = false;
			this->dataGridView5->AllowUserToDeleteRows = false;
			this->dataGridView5->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView5->Location = System::Drawing::Point(3, 25);
			this->dataGridView5->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView5->Name = L"dataGridView5";
			this->dataGridView5->ReadOnly = true;
			this->dataGridView5->RowTemplate->Height = 21;
			this->dataGridView5->Size = System::Drawing::Size(420, 283);
			this->dataGridView5->TabIndex = 2;
			// 
			// tabPage7
			// 
			this->tabPage7->Controls->Add(this->button_6);
			this->tabPage7->Controls->Add(this->textBox6);
			this->tabPage7->Controls->Add(this->dataGridView6);
			this->tabPage7->Location = System::Drawing::Point(4, 22);
			this->tabPage7->Name = L"tabPage7";
			this->tabPage7->Padding = System::Windows::Forms::Padding(3);
			this->tabPage7->Size = System::Drawing::Size(426, 310);
			this->tabPage7->TabIndex = 6;
			this->tabPage7->Text = L"\\6";
			this->tabPage7->UseVisualStyleBackColor = true;
			// 
			// button_6
			// 
			this->button_6->Location = System::Drawing::Point(349, 1);
			this->button_6->Name = L"button_6";
			this->button_6->Size = System::Drawing::Size(75, 23);
			this->button_6->TabIndex = 4;
			this->button_6->Text = L"検索";
			this->button_6->UseVisualStyleBackColor = true;
			this->button_6->Click += gcnew System::EventHandler(this, &Form1::button_6_Click);
			// 
			// textBox6
			// 
			this->textBox6->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox6->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox6->Location = System::Drawing::Point(3, 3);
			this->textBox6->Margin = System::Windows::Forms::Padding(0);
			this->textBox6->MaxLength = 100;
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(343, 19);
			this->textBox6->TabIndex = 3;
			// 
			// dataGridView6
			// 
			this->dataGridView6->AllowUserToAddRows = false;
			this->dataGridView6->AllowUserToDeleteRows = false;
			this->dataGridView6->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView6->Location = System::Drawing::Point(3, 25);
			this->dataGridView6->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView6->Name = L"dataGridView6";
			this->dataGridView6->ReadOnly = true;
			this->dataGridView6->RowTemplate->Height = 21;
			this->dataGridView6->Size = System::Drawing::Size(420, 283);
			this->dataGridView6->TabIndex = 2;
			// 
			// tabPage8
			// 
			this->tabPage8->Controls->Add(this->button_7);
			this->tabPage8->Controls->Add(this->textBox7);
			this->tabPage8->Controls->Add(this->dataGridView7);
			this->tabPage8->Location = System::Drawing::Point(4, 22);
			this->tabPage8->Name = L"tabPage8";
			this->tabPage8->Padding = System::Windows::Forms::Padding(3);
			this->tabPage8->Size = System::Drawing::Size(426, 310);
			this->tabPage8->TabIndex = 7;
			this->tabPage8->Text = L"\\7";
			this->tabPage8->UseVisualStyleBackColor = true;
			// 
			// button_7
			// 
			this->button_7->Location = System::Drawing::Point(349, 1);
			this->button_7->Name = L"button_7";
			this->button_7->Size = System::Drawing::Size(75, 23);
			this->button_7->TabIndex = 4;
			this->button_7->Text = L"検索";
			this->button_7->UseVisualStyleBackColor = true;
			this->button_7->Click += gcnew System::EventHandler(this, &Form1::button_7_Click);
			// 
			// textBox7
			// 
			this->textBox7->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox7->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox7->Location = System::Drawing::Point(3, 3);
			this->textBox7->Margin = System::Windows::Forms::Padding(0);
			this->textBox7->MaxLength = 100;
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(343, 19);
			this->textBox7->TabIndex = 3;
			// 
			// dataGridView7
			// 
			this->dataGridView7->AllowUserToAddRows = false;
			this->dataGridView7->AllowUserToDeleteRows = false;
			this->dataGridView7->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView7->Location = System::Drawing::Point(3, 25);
			this->dataGridView7->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView7->Name = L"dataGridView7";
			this->dataGridView7->ReadOnly = true;
			this->dataGridView7->RowTemplate->Height = 21;
			this->dataGridView7->Size = System::Drawing::Size(420, 283);
			this->dataGridView7->TabIndex = 2;
			// 
			// tabPage9
			// 
			this->tabPage9->Controls->Add(this->button_8);
			this->tabPage9->Controls->Add(this->textBox8);
			this->tabPage9->Controls->Add(this->dataGridView8);
			this->tabPage9->Location = System::Drawing::Point(4, 22);
			this->tabPage9->Name = L"tabPage9";
			this->tabPage9->Padding = System::Windows::Forms::Padding(3);
			this->tabPage9->Size = System::Drawing::Size(426, 310);
			this->tabPage9->TabIndex = 8;
			this->tabPage9->Text = L"\\8";
			this->tabPage9->UseVisualStyleBackColor = true;
			// 
			// button_8
			// 
			this->button_8->Location = System::Drawing::Point(349, 1);
			this->button_8->Name = L"button_8";
			this->button_8->Size = System::Drawing::Size(75, 23);
			this->button_8->TabIndex = 4;
			this->button_8->Text = L"検索";
			this->button_8->UseVisualStyleBackColor = true;
			this->button_8->Click += gcnew System::EventHandler(this, &Form1::button_8_Click);
			// 
			// textBox8
			// 
			this->textBox8->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox8->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox8->Location = System::Drawing::Point(3, 3);
			this->textBox8->Margin = System::Windows::Forms::Padding(0);
			this->textBox8->MaxLength = 100;
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(343, 19);
			this->textBox8->TabIndex = 3;
			// 
			// dataGridView8
			// 
			this->dataGridView8->AllowUserToAddRows = false;
			this->dataGridView8->AllowUserToDeleteRows = false;
			this->dataGridView8->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView8->Location = System::Drawing::Point(3, 25);
			this->dataGridView8->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView8->Name = L"dataGridView8";
			this->dataGridView8->ReadOnly = true;
			this->dataGridView8->RowTemplate->Height = 21;
			this->dataGridView8->Size = System::Drawing::Size(420, 283);
			this->dataGridView8->TabIndex = 2;
			// 
			// tabPage10
			// 
			this->tabPage10->Controls->Add(this->button_9);
			this->tabPage10->Controls->Add(this->textBox9);
			this->tabPage10->Controls->Add(this->dataGridView9);
			this->tabPage10->Controls->Add(this->textBox10);
			this->tabPage10->Location = System::Drawing::Point(4, 22);
			this->tabPage10->Name = L"tabPage10";
			this->tabPage10->Padding = System::Windows::Forms::Padding(3);
			this->tabPage10->Size = System::Drawing::Size(426, 310);
			this->tabPage10->TabIndex = 9;
			this->tabPage10->Text = L"\\9";
			this->tabPage10->UseVisualStyleBackColor = true;
			// 
			// button_9
			// 
			this->button_9->Location = System::Drawing::Point(349, 1);
			this->button_9->Name = L"button_9";
			this->button_9->Size = System::Drawing::Size(75, 23);
			this->button_9->TabIndex = 4;
			this->button_9->Text = L"検索";
			this->button_9->UseVisualStyleBackColor = true;
			this->button_9->Click += gcnew System::EventHandler(this, &Form1::button_9_Click);
			// 
			// textBox9
			// 
			this->textBox9->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->textBox9->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
			this->textBox9->Location = System::Drawing::Point(3, 3);
			this->textBox9->Margin = System::Windows::Forms::Padding(0);
			this->textBox9->MaxLength = 100;
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(343, 19);
			this->textBox9->TabIndex = 3;
			// 
			// dataGridView9
			// 
			this->dataGridView9->AllowUserToAddRows = false;
			this->dataGridView9->AllowUserToDeleteRows = false;
			this->dataGridView9->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView9->Location = System::Drawing::Point(3, 25);
			this->dataGridView9->Margin = System::Windows::Forms::Padding(0);
			this->dataGridView9->Name = L"dataGridView9";
			this->dataGridView9->ReadOnly = true;
			this->dataGridView9->RowTemplate->Height = 21;
			this->dataGridView9->Size = System::Drawing::Size(420, 283);
			this->dataGridView9->TabIndex = 2;
			// 
			// textBox10
			// 
			this->textBox10->Dock = System::Windows::Forms::DockStyle::Top;
			this->textBox10->Location = System::Drawing::Point(3, 3);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(420, 19);
			this->textBox10->TabIndex = 1;
			// 
			// statusStrip1
			// 
			this->statusStrip1->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->progressBar1, this->statusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 339);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(438, 22);
			this->statusStrip1->SizingGrip = false;
			this->statusStrip1->TabIndex = 2;
			// 
			// progressBar1
			// 
			this->progressBar1->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->progressBar1->Maximum = 1;
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(100, 16);
			this->progressBar1->Step = 1;
			// 
			// statusLabel1
			// 
			this->statusLabel1->AutoSize = false;
			this->statusLabel1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->statusLabel1->Name = L"statusLabel1";
			this->statusLabel1->Overflow = System::Windows::Forms::ToolStripItemOverflow::Never;
			this->statusLabel1->Size = System::Drawing::Size(50, 17);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(438, 361);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->tabControl1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->ShowIcon = false;
			this->Text = L"部品コード一覧";
			this->TopMost = true;
			this->Shown += gcnew System::EventHandler(this, &Form1::Form1_Shown);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->EndInit();
			this->tabPage4->ResumeLayout(false);
			this->tabPage4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView4))->EndInit();
			this->tabPage6->ResumeLayout(false);
			this->tabPage6->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView5))->EndInit();
			this->tabPage7->ResumeLayout(false);
			this->tabPage7->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView6))->EndInit();
			this->tabPage8->ResumeLayout(false);
			this->tabPage8->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView7))->EndInit();
			this->tabPage9->ResumeLayout(false);
			this->tabPage9->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView8))->EndInit();
			this->tabPage10->ResumeLayout(false);
			this->tabPage10->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView9))->EndInit();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
			 // ファイル名
	private: String^ fileName;

			 // OLEDB接続文字列
	private: String^ ConnectString;

			 // 各DataGridViewを纏める為
	private: array<DataGridView^>^ arrDgv;

			 // テーブル名
	private: array<String^>^  dbTableName;
	private: array<DataSet^>^ dtSet;
	private: array<Data::OleDb::OleDbDataAdapter^>^ dbAdapter;
	private: Data::OleDb::OleDbConnection^ dbConnect;

	private: System::Void Form1_Load(Object^ ender, EventArgs^ e)
			 {
				 fileName      = gcnew String((IO::Directory::GetCurrentDirectory()+"\\部品コード管理.mdb"));
				 ConnectString = gcnew String(L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=\""+fileName+L"\"");

				 // ファイルの有無の検査
				 if(!IO::File::Exists(fileName))
				 {
					 MessageBox::Show("実行ファイルと同じフォルダに\"部品コード管理.mdb\"が在りません。");
					 return;
				 }

				 dtSet	   = gcnew array<DataSet^>(9);
				 dbAdapter = gcnew array<Data::OleDb::OleDbDataAdapter^>(9);
				 dbConnect = gcnew Data::OleDb::OleDbConnection(ConnectString);

				 // テーブル名初期化
				 dbTableName = gcnew array<String^>(9){"T\\0", "T\\1", "T\\2", "T\\3", "T\\5", "T\\6", "T\\7", "T\\8", "T\\9"};

				 for(int i=0; i<9; i++)
				 {
					 dtSet[i] = gcnew DataSet(dbTableName[i]);
					 dbAdapter[i] = gcnew Data::OleDb::OleDbDataAdapter("SELECT * FROM "+dbTableName[i], dbConnect);
					 dbAdapter[i]->Fill(dtSet[i], dbTableName[i]);
				 }

				 // 各DataGridViewを纏める
				 arrDgv = gcnew array<DataGridView^>(9){this->dataGridView1, this->dataGridView2, this->dataGridView3,
														this->dataGridView4, this->dataGridView5, this->dataGridView6,
														this->dataGridView7, this->dataGridView8, this->dataGridView9};
				 for(int i=0; i<9; i++)
				 {
					 arrDgv[i]->MultiSelect = false;
				 }

				 // 各DataGridViewのデータソースに、各テーブルを関連付ける
				 for(int i=0; i<9; i++)
				 {
					 arrDgv[i]->DataSource = dtSet[i]->Tables[dbTableName[i]];
				 }

			 }

	private: System::Void Form1_Shown(System::Object^  sender, System::EventArgs^  e)
			 {
				 // オートコンプリートで使用する文字列を指定する為のもの
				 array<AutoCompleteStringCollection^>^ arrAutoCompleteStr = gcnew array<AutoCompleteStringCollection^>(9);

				 for(int i=0; i<9; i++)
				 {
					 arrAutoCompleteStr[i] = gcnew AutoCompleteStringCollection();
				 }

				 // オートコンプリートで使用する文字列を格納する為のもの
				 array<array<String^>^>^ arrPartsCode = gcnew array<array<String^>^>(9);

				 for(int i=0; i<9; i++)
				 {
					 arrPartsCode[i] = gcnew array<String^>(arrDgv[i]->Rows->Count);
					 
					 // オートコンプリートで使用する文字列を格納
					 dataGridViewCodeArray(arrDgv[i], arrPartsCode[i]);
				 }

				 for(int i=0; i<9; i++)
				 {
					 // オートコンプリートで使用する文字列として登録
					 arrAutoCompleteStr[i]->AddRange(arrPartsCode[i]);
				 }

				 // オートコンプリートで使用する登録した文字列を設定
				 this->textBox1->AutoCompleteCustomSource = arrAutoCompleteStr[0];
				 this->textBox2->AutoCompleteCustomSource = arrAutoCompleteStr[1];
				 this->textBox3->AutoCompleteCustomSource = arrAutoCompleteStr[2];
				 this->textBox4->AutoCompleteCustomSource = arrAutoCompleteStr[3];
				 this->textBox5->AutoCompleteCustomSource = arrAutoCompleteStr[4];
				 this->textBox6->AutoCompleteCustomSource = arrAutoCompleteStr[5];
				 this->textBox7->AutoCompleteCustomSource = arrAutoCompleteStr[6];
				 this->textBox8->AutoCompleteCustomSource = arrAutoCompleteStr[7];
				 this->textBox9->AutoCompleteCustomSource = arrAutoCompleteStr[8];
			 }


			 // コード検索(順次探索)
	private: System::Void dataGridViewText_tbTextComp(System::Windows::Forms::DataGridView^ dgv, String^ textBoxText)
			 {
				 dgv->MultiSelect = false;

				 const int count = dgv->Rows->Count;

				 if(textBoxText == String::Empty)
					 return;

				 this->progressBar1->Maximum = count;
				 this->progressBar1->Minimum = 0;
				 this->progressBar1->Step    = 1;
				 this->statusLabel1->Text    = L"検索中...";
				 this->statusLabel1->Invalidate();
				 this->Update();

				 for(int i=0; count > i; i++)
				 {
					 if((dgv->Rows[i]->Cells[0]->Value->ToString()) == textBoxText)
					 {
						 dgv->Rows[i]->Cells[0]->Selected = true;
						 break;
					 }
					 this->progressBar1->Value++;
				 }
				 this->progressBar1->Value = 0;
				 this->statusLabel1->Text = String::Empty;
			 }

			 // オートコンプリートで使用する文字列を格納
	private: System::Void dataGridViewCodeArray(DataGridView^ dgv, array<String^>^ arrCode)
			 {
				 const int count = dgv->Rows->Count;

				 for(int i=0; count > i; i++)
				 {
					 // 各行一列目(部品コード)のみ格納
					 arrCode[i] = dgv->Rows[i]->Cells[0]->Value->ToString();
				 }
			 }



			 // \0
	private: System::Void button_0_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 // 検索
				 dataGridViewText_tbTextComp(this->dataGridView1, this->textBox1->Text);
			 }
			 // \1
	private: System::Void button_1_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView2, this->textBox2->Text);
			 }
			 // \2
	private: System::Void button_2_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView3, this->textBox3->Text);
			 }
			 // \3
	private: System::Void button_3_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView4, this->textBox4->Text);
			 }
			 // \5
	private: System::Void button_5_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView5, this->textBox5->Text);
			 }
			 // \6
	private: System::Void button_6_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView6, this->textBox6->Text);
			 }
			 // \7
	private: System::Void button_7_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView7, this->textBox7->Text);
			 }
			 // \8
	private: System::Void button_8_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView8, this->textBox8->Text);
			 }
			 // \9
	private: System::Void button_9_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 dataGridViewText_tbTextComp(this->dataGridView9, this->textBox9->Text);
			 }
};
}

