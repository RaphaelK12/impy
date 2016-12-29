#include "im.h"
#include <stdio.h>


struct file_reader {
    im_reader base;
    FILE* fp;
};

struct file_writer {
    im_writer base;
    FILE* fp;
};



static size_t file_reader_read(im_reader* r, void* buf, size_t nbytes)
{
    struct file_reader *frdr = (struct file_reader*)r;
    int ret = fread(buf,1,nbytes,frdr->fp);
    if (ret<0) {
        im_err(ERR_FILE);
    }
    return ret;
}


static int file_reader_seek(im_reader* r, long pos, int whence)
{
    struct file_reader *fr = (struct file_reader*)r;
    int w;
    int ret;
    switch(whence) {
        case IM_SEEK_SET: w = SEEK_SET; break;
        case IM_SEEK_CUR: w = SEEK_CUR; break;
        default:
            im_err(ERR_BADPARAM);
            return -1;
    }
    ret = fseek(fr->fp,pos,w);
    if (ret<0) {
        im_err(ERR_FILE);
    }
    return ret;
}

static int file_reader_eof(im_reader* r)
{
    struct file_reader *fr = (struct file_reader*)r;
    return feof(fr->fp);
}


static int file_reader_close(im_reader* r)
{
    struct file_reader *fr = (struct file_reader*)r;
    if( fclose(fr->fp) == 0 ) {
        return 0;
    } else {
        im_err(ERR_FILE);
        return -1;
    }
}


im_reader* im_open_file_reader( const char* filename)
{
    struct file_reader *rdr = NULL;

    rdr = imalloc(sizeof(struct file_reader));
    if (!rdr) {
        im_err(ERR_NOMEM);
        return NULL;
    }

    rdr->fp = fopen(filename,"rb");
    if (!rdr->fp) {
        ifree(rdr);
        im_err(ERR_FILE);
        return NULL;
    }

    rdr->base.read = file_reader_read;
    rdr->base.seek = file_reader_seek;
    rdr->base.eof = file_reader_eof;
    rdr->base.close = file_reader_close;
    return (im_reader*)rdr;
}

int im_close_reader(im_reader* rdr) {
    int ret = rdr->close(rdr);
    ifree(rdr);
    return ret;
}


static size_t file_writer_write(im_writer* w, const void* buf, size_t nbytes)
{
    struct file_writer *fw = (struct file_writer*)w;
    int ret = fwrite(buf,1,nbytes,fw->fp);
    if (ret<0) {
        im_err(ERR_FILE);
    }
    return ret;
}

static int file_writer_close(im_writer* w)
{
    struct file_writer *fw = (struct file_writer*)w;
    if( fclose(fw->fp) == 0 ) {
        return 0;
    } else {
        im_err(ERR_FILE);
        return -1;
    }
}

im_writer* im_open_file_writer( const char* filename)
{
    struct file_writer *w = NULL;

    w = imalloc(sizeof(struct file_writer));
    if (!w) {
        im_err(ERR_NOMEM);
        return NULL;
    }

    w->fp = fopen(filename,"wb");
    if (!w->fp) {
        ifree(w);
        im_err(ERR_FILE);
        return NULL;
    }

    w->base.write = file_writer_write;
    w->base.close = file_writer_close;
    return (im_writer*)w;
}

int im_close_writer(im_writer* w)
{
    int ret = w->close(w);
    ifree(w);
    return ret;
}