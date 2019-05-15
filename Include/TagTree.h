#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include <vector>

#ifndef BUFF_STRING_SIZE;
#define BUFF_STRING_SIZE 32
#endif // !BUFF_STRING_SIZE;

using namespace std;

class Tag
{
public:
	Tag () :
		name_ (),
		data_ (),
		children_()
	{

	}

	string name_, data_;
	vector <Tag * > children_;
};

class TagTree 
{
public:
	TagTree () :
		tagMap_ (),
		root_ (nullptr)
	{
	}

	~TagTree ()
	{
		deleteTag (root_);
	}

	bool loadFromFile (FILE * file)
	{
		const int strSize = BUFF_STRING_SIZE;
		char buff[strSize];

		fscanf_s (file, " %s", buff, strSize);
		if (string (buff) != "<") return false;

		root_ = readTag (file);

		vector <string> path;
		fillMap (root_, path);

		return true;
	}

	bool loadFromFile (string filename)
	{
		FILE * file = fopen (filename.c_str (), "r");
		if (!file) return false;

		bool success = loadFromFile (file);

		fclose (file);

		return success;
	}

	bool saveToFile (FILE * file)
	{
		printTag (file, root_);

		return true;
	}

	bool saveToFile (string filename)
	{
		FILE * file = fopen (filename.c_str (), "w");
		if (!file) return false;

		bool success = saveToFile (file);

		fclose (file);

		return success;
	}

	string getDataByPath (string path, string defaultData = "")
	{
		vector <string> pathArray;

		string tagName;

		for (char i : path)
		{
			if (i == '/')
			{
				pathArray.push_back (tagName);
				tagName.clear ();
				continue;
			}

			tagName += i;
		}

		if (tagName != "") pathArray.push_back (tagName);

		if (tagMap_.find (pathArray) == tagMap_.end ())
		{
			setDataByPath (path, defaultData);
			return defaultData;
		}

		return tagMap_.at (pathArray);
	}

	bool setDataByPath (string path, string data)
	{
		vector <string> pathArray;

		string tagName;

		for (char i : path)
		{
			if (i == '/')
			{
				pathArray.push_back (tagName);
				tagName.clear ();
				continue;
			}

			tagName += i;
		}

		if (tagName != "") pathArray.push_back (tagName);

		tagMap_.insert (pair <vector <string>, string> (pathArray, data));
		addTag (root_, pathArray, data);

		return true;
	}

private:
	Tag * readTag (FILE * file)
	{
		Tag * tag = new Tag;

		const int strSize = BUFF_STRING_SIZE;
		char buff[strSize];

		fscanf_s (file, " %s", buff, strSize);
		tag->name_ = buff;
		fscanf_s (file, " %s", buff, strSize);

		if (string (buff) != ">")
		{
			fscanf_s (file, " =");
			fscanf_s (file, " \"%[^\"]\"", buff, strSize);
			tag->data_ = buff;
			fscanf_s (file, " >");
		}

		while (true)
		{
			fscanf_s (file, " %s", buff, strSize);
			if (string (buff) == "<")
				tag->children_.push_back (readTag (file));
			else
				break;
		}

		fscanf_s (file, " %s %s", buff, strSize, buff, strSize);

		return tag;
	}

	void printTag (FILE * file, Tag * tag, int level = 0)
	{
		if (!file || !tag) return;

		for (int i = 0; i < level; i++)
			fprintf (file, "\t");

		fprintf (file, "< ");
		fprintf (file, "%s", tag->name_.c_str ());
		if (tag->data_ != "")
			fprintf (file, " value = \"%s\"", tag->data_.c_str ());
		fprintf (file, " >\n");

		for (Tag * el : tag->children_)
			printTag (file, el, level + 1);

		for (int i = 0; i < level; i++)
			fprintf (file, "\t");
		fprintf (file, "</ %s >\n", tag->name_.c_str ());
	}

	void fillMap (Tag * tag, vector <string> & path)
	{
		path.push_back (tag->name_);

		if (tag->data_ != "")
		{
			tagMap_.insert (pair <vector <string>, string> (path, tag->data_));
		}

		for (Tag * el : tag->children_)
			fillMap (el, path);

		path.pop_back ();
	}

	void deleteTag (Tag * tag)
	{
		for (Tag * el : tag->children_)
			deleteTag (el);

		delete (tag);
	}

	void addTag (Tag * tag, vector <string> & path, string val, int iterator = 0)
	{
		if (iterator == path.size () - 1) return;

		for (Tag * el : tag->children_)
		{
			if (el->name_ == path[iterator + 1])
			{
				addTag (el, path, val, iterator + 1);
				if (iterator == 1)
					el->data_ = val;
				return;
			}
		}

		Tag * next = new Tag;
		next->name_ = path[iterator + 1];
		if (iterator == 1)
			next->data_ = val;

		tag->children_.push_back (next);
		addTag (next, path, val, iterator + 1);
	}

	map <vector <string>, string> tagMap_;
	Tag * root_;
};

